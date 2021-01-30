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


using Calculation::DataTable;
using Calculation::DataList;
using Calculation::Data;
using Calculation::MemberType_t;
using Calculation::ResponseType_t;

ThemeWidget::ThemeWidget(QWidget *parent) :
    QWidget(parent),
    m_listCount(1),
    m_valueMax(10),
    m_valueCount(10),
    main_chart(new QChart()),
    _whichMemberIsPicked(MemberType_t::Proportional),
    _whichResponseIsPicked(ResponseType_t::Step),
    m_dataTable(generateRandomData(m_listCount, 0, 0)), //FIXME it's probably not needed
    m_ui(new Ui_ThemeWidgetForm)
{
    m_ui->setupUi(this);
    populateThemeBox();
    populateResponseTypeBox();
    populateMemberTypeBox();

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

    //Make line editing accept only numbers
    m_ui->kLineEdit->setValidator(new QIntValidator(0, 100, this));
    m_ui->t1LineEdit->setValidator(new QIntValidator(0, 100, this));
    m_ui->t2LineEdit->setValidator(new QIntValidator(0, 100, this));
    m_ui->t3LineEdit->setValidator(new QIntValidator(0, 100, this));
    m_ui->t4LineEdit->setValidator(new QIntValidator(0, 100, this));

    m_ui->t1LineEdit->setEnabled(false);
    m_ui->t2LineEdit->setEnabled(false);
    m_ui->t3LineEdit->setEnabled(false);
    m_ui->t4LineEdit->setEnabled(false);

    connect(m_ui->memberTypeComboBox,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(memberChangedCallback(int)));

    connect(m_ui->signalTypeComboBox,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(responseChangedCallback(int)));

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
    m_ui->themeComboBox->addItem(tr("Light"),           QChart::ChartThemeLight);
    m_ui->themeComboBox->addItem(tr("Blue Cerulean"),   QChart::ChartThemeBlueCerulean);
    m_ui->themeComboBox->addItem(tr("Dark"),            QChart::ChartThemeDark);
    m_ui->themeComboBox->addItem(tr("Brown Sand"),      QChart::ChartThemeBrownSand);
    m_ui->themeComboBox->addItem(tr("Blue NCS"),        QChart::ChartThemeBlueNcs);
    m_ui->themeComboBox->addItem(tr("High Contrast"),   QChart::ChartThemeHighContrast);
    m_ui->themeComboBox->addItem(tr("Blue Icy"),        QChart::ChartThemeBlueIcy);
    m_ui->themeComboBox->addItem(tr("Qt"),              QChart::ChartThemeQt);
}

void ThemeWidget::populateResponseTypeBox()
{
    using Calculation::ResponseType_t;

    m_ui->signalTypeComboBox->addItem(tr("Step"),    ResponseType_t::Step);
    m_ui->signalTypeComboBox->addItem(tr("Impulse"), ResponseType_t::Impulse);
}

void ThemeWidget::populateMemberTypeBox()
{
    using Calculation::MemberType_t;

    m_ui->memberTypeComboBox->addItem(tr("Proportional"),           MemberType_t::Proportional);
    m_ui->memberTypeComboBox->addItem(tr("Inertion First Order"),   MemberType_t::InertionFirstOrder);
    m_ui->memberTypeComboBox->addItem(tr("Inertion Second Order"),  MemberType_t::InertionFourthOrder);
    m_ui->memberTypeComboBox->addItem(tr("Inertion Third Order"),   MemberType_t::InertionThirdOrder);
    m_ui->memberTypeComboBox->addItem(tr("Inertion Fourth Order"),  MemberType_t::InertionFourthOrder);
    m_ui->memberTypeComboBox->addItem(tr("Integration"),            MemberType_t::Integration);
    m_ui->memberTypeComboBox->addItem(tr("Differentiation"),        MemberType_t::Differentiation);
}

QChart *ThemeWidget::createSplineChart() const
{
    this->main_chart->setTitle(tr("ChartNameBasedOnTypeEntered")); //TODO add it
    QString name(tr("Equation: ")); //TODO make it automatic
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
    _data.k  = m_ui->kLineEdit->text().toInt();
    _data.t1 = m_ui->t1LineEdit->text().toInt();
    _data.t2 = m_ui->t2LineEdit->text().toInt();
    _data.t3 = m_ui->t3LineEdit->text().toInt();
    _data.t4 = m_ui->t4LineEdit->text().toInt();

    auto result = this->calculate(_data);

    //FIXME make it call ,,setBorderValues()" from here
    this->main_chart->axes(Qt::Horizontal).first()->setRange(0, _data.maxXValue * 1.2);
    this->main_chart->axes(Qt::Vertical).first()->setRange(0, _data.maxYValue * 1.2);

    this->updateChart(result);
}

void ThemeWidget::updateChart(DataTable dataTable)
{
    this->main_chart->setTitle(tr("Spline chart")); //TODO that name can be taken from &data
    this->main_chart->removeAllSeries();
    QString name(tr("Series "));
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
    using Calculation::MemberType_t;

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
    using Calculation::MemberType_t;

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

void ThemeWidget::responseChangedCallback(int index)
{
    this->_whichResponseIsPicked = static_cast<Calculation::ResponseType_t>(index+1);
}


//TODO this function should be moved to calculation.cpp
DataTable ThemeWidget::calculate(Calculation::DataAcquired_t& data)
{
    DataTable result;
    DataList dataList;
    std::vector<qreal> xValVector;
    std::vector<qreal> yValVector;

    data.memberType = this->_whichMemberIsPicked;       //This shouldn't be here, refactor
    data.responseType = this->_whichResponseIsPicked;

    {
        qreal yValue(0);
        for (int t(-10); t < m_valueCount; t++) {   //TODO hardcoded temporary
            yValue = this->_calculator.calculate(data, t);
            QPointF value((qreal) t, yValue);
            QString label = "Slice " + QString::number(0) + ":" + QString::number(t);
            dataList << Data(value, label);
            xValVector.push_back(t);
            yValVector.push_back(yValue);
        }
        result << dataList;
    }

    this->setBorderValues(data, xValVector, yValVector);

    return result;
}

void ThemeWidget::setBorderValues(Calculation::DataAcquired_t& data,    //TODO move it to calculations namespace
                                  std::vector<qreal> xValVector,
                                  std::vector<qreal> yValVector)
{

    data.minXValue = *std::min_element(xValVector.begin(), xValVector.end());
    data.maxXValue = *std::max_element(xValVector.begin(), xValVector.end());
    data.minYValue = *std::min_element(yValVector.begin(), yValVector.end());
    data.maxYValue = *std::max_element(yValVector.begin(), yValVector.end());

    return;
}

