#include "themewidget.h"
#include "ui_themewidget.h"

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QAbstractBarSeries>
#include <QtCharts/QPercentBarSeries>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QLegend>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtCore/QRandomGenerator>
#include <QtCharts/QBarCategoryAxis>
#include <QtWidgets/QApplication>
#include <QtCharts/QValueAxis>

using Drawer::DataTable;
using Drawer::DataList;
using Drawer::Data;

ThemeWidget::ThemeWidget(QWidget *parent) :
    QWidget(parent),
    m_listCount(1),
    m_valueMax(10),
    m_valueCount(7),
    main_chart(new QChart()),
    m_dataTable(generateRandomData(m_listCount, 0, 0)),
    m_ui(new Ui_ThemeWidgetForm)
{
    m_ui->setupUi(this);
    populateThemeBox();
    populateResponseTypeBox();
    populateMemberTypeBox();

    //create charts

    QChartView *chartView;

    chartView = new QChartView(createSplineChart());
    m_ui->gridLayout->addWidget(chartView, 1, 0);
    m_charts << chartView;

    //create button
    m_ui->equationPushButton->setDefault(false);
    connect(m_ui->equationPushButton,
            &QAbstractButton::clicked,
            this,
            &ThemeWidget::showGraphGotPressed);

    //create text box
    m_ui->equationPushButton->setEnabled(false);
    connectCallbackToPushButton();
    m_ui->t1LineEdit->setEnabled(false);
    m_ui->t2LineEdit->setEnabled(false);
    m_ui->t3LineEdit->setEnabled(false);
    m_ui->t4LineEdit->setEnabled(false);

    connect(m_ui->memberTypeComboBox,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(memberChangedCallback(int)));


    // Set the colors from the light theme as default ones
    QPalette pal = qApp->palette();
    pal.setColor(QPalette::Window, QRgb(0xf0f0f0));
    pal.setColor(QPalette::WindowText, QRgb(0x404044));
    qApp->setPalette(pal);

    updateUI();
}

ThemeWidget::~ThemeWidget()
{
    delete m_ui;
}

DataTable ThemeWidget::generateRandomData(int listCount, int valueMax, int valueCount) const
{
    DataTable dataTable;

    // generate random data
    for (int i(0); i < listCount; i++) {
        DataList dataList;
        qreal yValue(0);
        for (int j(0); j < valueCount; j++) {
            yValue = yValue + QRandomGenerator::global()->bounded(valueMax / (qreal) valueCount);
            QPointF value((j + QRandomGenerator::global()->generateDouble()) * ((qreal) m_valueMax / (qreal) valueCount),
                          yValue);
            QString label = "Slice " + QString::number(i) + ":" + QString::number(j);
            dataList << Data(value, label);
        }
        dataTable << dataList;
    }

    return dataTable;
}

void ThemeWidget::populateThemeBox()
{
    // add items to theme combobox
    m_ui->themeComboBox->addItem("Light",           QChart::ChartThemeLight);
    m_ui->themeComboBox->addItem("Blue Cerulean",   QChart::ChartThemeBlueCerulean);
    m_ui->themeComboBox->addItem("Dark",            QChart::ChartThemeDark);
    m_ui->themeComboBox->addItem("Brown Sand",      QChart::ChartThemeBrownSand);
    m_ui->themeComboBox->addItem("Blue NCS",        QChart::ChartThemeBlueNcs);
    m_ui->themeComboBox->addItem("High Contrast",   QChart::ChartThemeHighContrast);
    m_ui->themeComboBox->addItem("Blue Icy",        QChart::ChartThemeBlueIcy);
    m_ui->themeComboBox->addItem("Qt",              QChart::ChartThemeQt);
}

void ThemeWidget::populateResponseTypeBox()
{
    using Drawer::ResponseType_t;

    m_ui->signalTypeComboBox->addItem("Step",    ResponseType_t::Step);
    m_ui->signalTypeComboBox->addItem("Impulse", ResponseType_t::Impulse);
}

void ThemeWidget::populateMemberTypeBox()
{
    using Drawer::MemberType_t;

    m_ui->memberTypeComboBox->addItem("Proportional",           MemberType_t::Proportional);
    m_ui->memberTypeComboBox->addItem("Inertion First Order",   MemberType_t::InertionFirstOrder);
    m_ui->memberTypeComboBox->addItem("Inertion Second Order",  MemberType_t::InertionFourthOrder);
    m_ui->memberTypeComboBox->addItem("Inertion Third Order",   MemberType_t::InertionThirdOrder);
    m_ui->memberTypeComboBox->addItem("Inertion Fourth Order",  MemberType_t::InertionFourthOrder);
    m_ui->memberTypeComboBox->addItem("Integration",            MemberType_t::Integration);
    m_ui->memberTypeComboBox->addItem("Differentiation",        MemberType_t::Differentiation);
}

QChart *ThemeWidget::createSplineChart() const
{
//    QChart *chart = new QChart();
    this->main_chart->setTitle("ChartNameBasedOnTypeEntered"); //TODO add it
    QString name("Equation: ");
    int nameIndex = 0;
    for (const DataList &list : m_dataTable) {
        QSplineSeries *series = new QSplineSeries(this->main_chart);
        for (const Data &data : list)
            series->append(data.first);
        series->setName(name + QString::number(nameIndex));
        nameIndex++;
        this->main_chart->addSeries(series);
    }

    this->main_chart->createDefaultAxes();
    this->main_chart->axes(Qt::Horizontal).first()->setRange(0, m_valueMax);
    this->main_chart->axes(Qt::Vertical).first()->setRange(0, m_valueCount);

    // Add space to label to add space between labels and axis
    QValueAxis *axisY = qobject_cast<QValueAxis*>(this->main_chart->axes(Qt::Vertical).first());
    Q_ASSERT(axisY);
    axisY->setLabelFormat("%.1f  ");
    return this->main_chart;
}

void ThemeWidget::updateUI()
{
    //![6]
    QChart::ChartTheme theme = static_cast<QChart::ChartTheme>(
                m_ui->themeComboBox->itemData(m_ui->themeComboBox->currentIndex()).toInt());
    //![6]
    const auto charts = m_charts;
    if (!m_charts.isEmpty() && m_charts.at(0)->chart()->theme() != theme) {
        for (QChartView *chartView : charts) {
            //![7]
            chartView->chart()->setTheme(theme);
            //![7]
        }

        // Set palette colors based on selected theme
        //![8]
        QPalette pal = window()->palette();
        if (theme == QChart::ChartThemeLight) {
            pal.setColor(QPalette::Window, QRgb(0xf0f0f0));
            pal.setColor(QPalette::WindowText, QRgb(0x404044));
        //![8]
        } else if (theme == QChart::ChartThemeDark) {
            pal.setColor(QPalette::Window, QRgb(0x121218));
            pal.setColor(QPalette::WindowText, QRgb(0xd6d6d6));
        } else if (theme == QChart::ChartThemeBlueCerulean) {
            pal.setColor(QPalette::Window, QRgb(0x40434a));
            pal.setColor(QPalette::WindowText, QRgb(0xd6d6d6));
        } else if (theme == QChart::ChartThemeBrownSand) {
            pal.setColor(QPalette::Window, QRgb(0x9e8965));
            pal.setColor(QPalette::WindowText, QRgb(0x404044));
        } else if (theme == QChart::ChartThemeBlueNcs) {
            pal.setColor(QPalette::Window, QRgb(0x018bba));
            pal.setColor(QPalette::WindowText, QRgb(0x404044));
        } else if (theme == QChart::ChartThemeHighContrast) {
            pal.setColor(QPalette::Window, QRgb(0xffab03));
            pal.setColor(QPalette::WindowText, QRgb(0x181818));
        } else if (theme == QChart::ChartThemeBlueIcy) {
            pal.setColor(QPalette::Window, QRgb(0xcee7f0));
            pal.setColor(QPalette::WindowText, QRgb(0x404044));
        } else {
            pal.setColor(QPalette::Window, QRgb(0xf0f0f0));
            pal.setColor(QPalette::WindowText, QRgb(0x404044));
        }
        window()->setPalette(pal);
    }


    for (QChartView *chartView : charts)
    {
        chartView->chart()->legend()->setAlignment(Qt::AlignLeft);
        chartView->chart()->legend()->show();
    }
}

void ThemeWidget::showGraphGotPressed()
{
    //TODO This is where we get when ,,show graph" get's pressed. Handle it, mr Pedro
    //f.e.:
    //auto inputFromTextBox = m_ui->equationLineEdit->text();
    //auto data = ThemeWidget::calculateSomeStuff(inputFromTextBox);
    //ThemeWidget::updateChart(data)
    //That's how I suggest solving this. Single function, single responsibility.

    auto data = this->generateRandomData(this->m_listCount,
                                         this->m_valueMax,
                                         this->m_valueCount);
    this->updateChart(data);
}

void ThemeWidget::updateChart(DataTable dataTable)
{
    this->main_chart->setTitle("Spline chart");    //TODO Pedro, you can set whatever name you want based on what equation we get
    this->main_chart->removeAllSeries();
    QString name("Series ");
    int nameIndex = 0;
    for (const DataList &list : dataTable) {
        QSplineSeries *series = new QSplineSeries(this->main_chart);
        for (const Data &data : list)
            series->append(data.first);
        series->setName(name + QString::number(nameIndex));
        nameIndex++;
        this->main_chart->addSeries(series);
    }
}

void ThemeWidget::enableShowGraphButton()
{
    m_ui->equationPushButton->setEnabled(this->isAllDataProvided());
}

bool ThemeWidget::isAllDataProvided()
{
    using Drawer::MemberType_t;

    bool result = false;

    switch(this->_whichMemberIsPicked)
    {
    case MemberType_t::Proportional:
        result = ! this->m_ui->kLineEdit->text().isEmpty();
        break;
    case MemberType_t::InertionFirstOrder:
        result = ! ( this->m_ui->kLineEdit->text().isEmpty()  ||
                     this->m_ui->t1LineEdit->text().isEmpty() );
        break;
    case MemberType_t::InertionSecondOrder:
        result = ! ( this->m_ui->kLineEdit->text().isEmpty()  ||
                     this->m_ui->t1LineEdit->text().isEmpty() ||
                     this->m_ui->t2LineEdit->text().isEmpty() );
        break;
    case MemberType_t::InertionThirdOrder:
        result = ! ( this->m_ui->kLineEdit->text().isEmpty()  ||
                     this->m_ui->t1LineEdit->text().isEmpty() ||
                     this->m_ui->t2LineEdit->text().isEmpty() ||
                     this->m_ui->t3LineEdit->text().isEmpty() );
        break;
    case MemberType_t::InertionFourthOrder:

        result = ! ( this->m_ui->kLineEdit->text().isEmpty()  ||
                     this->m_ui->t1LineEdit->text().isEmpty() ||
                     this->m_ui->t2LineEdit->text().isEmpty() ||
                     this->m_ui->t3LineEdit->text().isEmpty() ||
                     this->m_ui->t4LineEdit->text().isEmpty() );
        break;
    case MemberType_t::Integration:
        result = ! ( this->m_ui->kLineEdit->text().isEmpty() );
        break;
    case MemberType_t::Differentiation:
        result = ! ( this->m_ui->kLineEdit->text().isEmpty() );
        break;

    default:
        break;
    }

    return result;
}

void ThemeWidget::connectCallbackToPushButton()
{
    connect(m_ui->kLineEdit,
            &QLineEdit::textChanged,
            this,
            &ThemeWidget::enableShowGraphButton);

    connect(m_ui->t1LineEdit,
            &QLineEdit::textChanged,
            this,
            &ThemeWidget::enableShowGraphButton);

    connect(m_ui->t2LineEdit,
            &QLineEdit::textChanged,
            this,
            &ThemeWidget::enableShowGraphButton);

    connect(m_ui->t3LineEdit,
            &QLineEdit::textChanged,
            this,
            &ThemeWidget::enableShowGraphButton);

    connect(m_ui->t4LineEdit,
            &QLineEdit::textChanged,
            this,
            &ThemeWidget::enableShowGraphButton);


    connect(m_ui->memberTypeComboBox,
            &QComboBox::currentTextChanged,
            this,
            &ThemeWidget::enableShowGraphButton);
}

void ThemeWidget::memberChangedCallback(int index)
{
    using Drawer::MemberType_t;

    this->_whichMemberIsPicked = static_cast<MemberType_t>(index+1);

    switch(this->_whichMemberIsPicked)
    {
    case MemberType_t::Proportional:
    case MemberType_t::Integration:
    case MemberType_t::Differentiation:
        m_ui->t1LineEdit->setEnabled(false);
        m_ui->t2LineEdit->setEnabled(false);
        m_ui->t3LineEdit->setEnabled(false);
        m_ui->t4LineEdit->setEnabled(false);

        m_ui->t1LineEdit->clear();
        m_ui->t2LineEdit->clear();
        m_ui->t3LineEdit->clear();
        m_ui->t4LineEdit->clear();
        break;

    case MemberType_t::InertionFirstOrder:
        m_ui->t1LineEdit->setEnabled(true);
        m_ui->t2LineEdit->setEnabled(false);
        m_ui->t3LineEdit->setEnabled(false);
        m_ui->t4LineEdit->setEnabled(false);

        m_ui->t2LineEdit->clear();
        m_ui->t3LineEdit->clear();
        m_ui->t4LineEdit->clear();
        break;

    case MemberType_t::InertionSecondOrder:
        m_ui->t1LineEdit->setEnabled(true);
        m_ui->t2LineEdit->setEnabled(true);
        m_ui->t3LineEdit->setEnabled(false);
        m_ui->t4LineEdit->setEnabled(false);

        m_ui->t3LineEdit->clear();
        m_ui->t4LineEdit->clear();
        break;

    case MemberType_t::InertionThirdOrder:
        m_ui->t1LineEdit->setEnabled(true);
        m_ui->t2LineEdit->setEnabled(true);
        m_ui->t3LineEdit->setEnabled(true);
        m_ui->t4LineEdit->setEnabled(false);

        m_ui->t4LineEdit->clear();
        break;

    case MemberType_t::InertionFourthOrder:
        m_ui->t1LineEdit->setEnabled(true);
        m_ui->t2LineEdit->setEnabled(true);
        m_ui->t3LineEdit->setEnabled(true);
        m_ui->t4LineEdit->setEnabled(true);
        break;

    default:
        break;
    }
}
