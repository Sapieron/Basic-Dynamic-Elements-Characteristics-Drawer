#include "themewidget.h"
#include "ui_themewidget.h"
#include <utils/chartFormatter.hpp>

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
#include <QtCharts/QPolarChart>


using Calculation::DataTable;
using Calculation::DataList;
using Calculation::Data;
using Calculation::CharacteristicType_t;
using Calculation::MemberType_t;
using Calculation::ResponseType_t;

QT_CHARTS_USE_NAMESPACE

ThemeWidget::ThemeWidget(QWidget *parent) :
    QWidget(parent),
    m_listCount(1),
    m_valueMax(10),
    m_valueCount(10),
    main_chart(new QChart()),
    _whichMemberIsPicked(MemberType_t::Proportional),
    _whichResponseIsPicked(ResponseType_t::Step),
    _whichCharactersiticIsPicked(CharacteristicType_t::Time),
    m_dataTable(generateRandomData(m_listCount, 0, 0)), //FIXME initialize data here to 0
    m_ui(new Ui_ThemeWidgetForm)
{
    m_ui->setupUi(this);
    this->populateThemeBox();
    this->populateResponseTypeBox();
    this->populateMemberTypeBox();
    this->populateCharacteristicTypeBox();

//    _chartView = new QChartView_scaledAxis(); //TODO Didn't work and program works extremely slow, as it anaylyzes all series all the time - can be optimizied easly
    _chartView = new QChartView();
    _chartView->setChart(createSplineChart());

    m_ui->gridLayout->addWidget(_chartView, 1, 0);
    m_charts << _chartView;

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
    m_ui->kLineEdit->setValidator(new QDoubleValidator(0.0, 100.0, 10, this)); //TODO move it to separate function
    m_ui->t1LineEdit->setValidator(new QDoubleValidator(0.0, 100.0, 10, this));
    m_ui->t2LineEdit->setValidator(new QDoubleValidator(0.0, 100.0, 10, this));
    m_ui->t3LineEdit->setValidator(new QDoubleValidator(0.0, 100.0, 10, this));
    m_ui->t4LineEdit->setValidator(new QDoubleValidator(0.0, 100.0, 10, this));

    m_ui->kpLineEdit->setValidator(new QDoubleValidator(-100.0, 100.0, 10, this));
    m_ui->kiLineEdit->setValidator(new QDoubleValidator(-100.0, 100.0, 10, this));
    m_ui->kdLineEdit->setValidator(new QDoubleValidator(-100.0, 100.0, 10, this));
    m_ui->dtLineEdit->setValidator(new QDoubleValidator(0.0, 100.0, 10, this));
    m_ui->targetLineEdit->setValidator(new QDoubleValidator(-100.0, 100.0, 10, this));
    m_ui->startLineEdit->setValidator(new QDoubleValidator(-100.0, 100.0, 10, this));
    m_ui->maxTLineEdit->setValidator(new QIntValidator(0, 100, this));

    m_ui->t1LineEdit->setEnabled(false);
    m_ui->t2LineEdit->setEnabled(false);
    m_ui->t3LineEdit->setEnabled(false);
    m_ui->t4LineEdit->setEnabled(false);
    m_ui->tDlineEdit->setEnabled(false);

    m_ui->kpLineEdit->setVisible(false);
    m_ui->kiLineEdit->setVisible(false);
    m_ui->kdLineEdit->setVisible(false);
    m_ui->dtLineEdit->setVisible(false);
    m_ui->targetLineEdit->setVisible(false);
    m_ui->startLineEdit->setVisible(false);

    m_ui->kpLabel->setVisible(false);
    m_ui->kiLabel->setVisible(false);
    m_ui->kdLabel->setVisible(false);
    m_ui->dtLabel->setVisible(false);
    m_ui->targetLabel->setVisible(false);
    m_ui->startLabel->setVisible(false);

    m_ui->maxTLineEdit->setText("100");

    connect(m_ui->memberTypeComboBox,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(memberChangedCallback(int)));

    connect(m_ui->signalTypeComboBox,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(responseChangedCallback(int)));

    connect(m_ui->characteristicComboBox,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(characteristicChangedCallback(int)));

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

DataTable ThemeWidget::generateRandomData(int listCount, int valueMax, int valueCount) const    //FIXME it's not needed, m_data should be set to 0
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

void ThemeWidget::populateCharacteristicTypeBox()
{
    using Calculation::CharacteristicType_t;

    m_ui->characteristicComboBox->addItem(tr("Time"),            CharacteristicType_t::Time);
    m_ui->characteristicComboBox->addItem(tr("Amplitude-Phase"), CharacteristicType_t::AmplitudePhase);
    m_ui->characteristicComboBox->addItem(tr("PID"),             CharacteristicType_t::PID);
}

QChart *ThemeWidget::createSplineChart() const  //FIXME it's probably not needed
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
    this->main_chart->axes(Qt::Horizontal).first()->setRange(-1, 1);
    this->main_chart->axes(Qt::Vertical).first()->setRange(-1, 1);

    // Add space to label to add space between labels and axis
    QValueAxis *axisY = qobject_cast<QValueAxis*>(this->main_chart->axes(Qt::Vertical).first());
    Q_ASSERT(axisY);
    axisY->setLabelFormat("%.3f  ");

    this->main_chart->axes(Qt::Horizontal).first()->setTitleText(tr("t[s]"));
    this->main_chart->axes(Qt::Vertical).first()->setTitleText(tr("h(t)"));

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
    _data.k  = m_ui->kLineEdit->text().toDouble();
    _data.t1 = m_ui->t1LineEdit->text().toDouble();
    _data.t2 = m_ui->t2LineEdit->text().toDouble();
    _data.t3 = m_ui->t3LineEdit->text().toDouble();
    _data.t4 = m_ui->t4LineEdit->text().toDouble();
    _data.td = m_ui->tDlineEdit->text().toDouble();

    _data.kp         = m_ui->kpLineEdit->text().toDouble();
    _data.ki         = m_ui->kiLineEdit->text().toDouble();
    _data.kd         = m_ui->kdLineEdit->text().toDouble();
    _data.dt         = m_ui->dtLineEdit->text().toDouble();
    _data.target     = m_ui->targetLineEdit->text().toDouble();
    _data.startPoint = m_ui->startLineEdit->text().toDouble();

    _data.maxT       = m_ui->maxTLineEdit->text().toInt();

    auto result = this->calculate(_data);

    this->updateChart(result);
}

void ThemeWidget::updateChart(DataTable dataTable)  //TODO maybe ,,updateSplineData" is a better name?
{
    this->main_chart->setTitle(tr("Spline chart")); //TODO that name can be taken from &data
    this->main_chart->removeAllSeries();
    QString name(tr("Function "));
    int nameIndex = 0;
    for (const DataList &list : dataTable) {
        QSplineSeries *series = new QSplineSeries(this->main_chart);
        for (const Data &data : list)
            series->append(data.first);
        series->setName(name + QString::number(nameIndex));
        nameIndex++;

        this->main_chart->addSeries(series);
        this->main_chart->createDefaultAxes();
    }
}

void ThemeWidget::enableShowGraphButton()
{
    m_ui->equationPushButton->setEnabled(this->isAllDataProvided());
}

bool ThemeWidget::isAllDataProvided()
{
    bool result;

    switch(_whichCharactersiticIsPicked)
    {
    case CharacteristicType_t::Time:
    case CharacteristicType_t::AmplitudePhase:
        result = this->isAllDataProvided_TimePhaseAmplitude();
        break;

    case CharacteristicType_t::PID:
        result = this->isAllDataProvided_PID();
        break;
    }

    return result;
}

bool ThemeWidget::isAllDataProvided_TimePhaseAmplitude()
{
    using Calculation::MemberType_t;

    bool result = false;

    switch(this->_whichMemberIsPicked)
    {
    case MemberType_t::Proportional:
        result = ! ( this->m_ui->kLineEdit->text().isEmpty()  ||
                     this->m_ui->maxTLineEdit->text().isEmpty() );
        break;
    case MemberType_t::InertionFirstOrder:
        result = ! ( this->m_ui->kLineEdit->text().isEmpty()  ||
                     this->m_ui->t1LineEdit->text().isEmpty() ||
                     this->m_ui->maxTLineEdit->text().isEmpty() );
        break;
    case MemberType_t::InertionSecondOrder:
        result = ! ( this->m_ui->kLineEdit->text().isEmpty()  ||
                     this->m_ui->t1LineEdit->text().isEmpty() ||
                     this->m_ui->t2LineEdit->text().isEmpty() ||
                     this->m_ui->maxTLineEdit->text().isEmpty() );
        break;
    case MemberType_t::InertionThirdOrder:
        result = ! ( this->m_ui->kLineEdit->text().isEmpty()  ||
                     this->m_ui->t1LineEdit->text().isEmpty() ||
                     this->m_ui->t2LineEdit->text().isEmpty() ||
                     this->m_ui->t3LineEdit->text().isEmpty() ||
                     this->m_ui->maxTLineEdit->text().isEmpty() );
        break;
    case MemberType_t::InertionFourthOrder:

        result = ! ( this->m_ui->kLineEdit->text().isEmpty()  ||
                     this->m_ui->t1LineEdit->text().isEmpty() ||
                     this->m_ui->t2LineEdit->text().isEmpty() ||
                     this->m_ui->t3LineEdit->text().isEmpty() ||
                     this->m_ui->t4LineEdit->text().isEmpty() ||
                     this->m_ui->maxTLineEdit->text().isEmpty() );
        break;
    case MemberType_t::Integration:
        result = ! ( this->m_ui->kLineEdit->text().isEmpty()  ||
                     this->m_ui->maxTLineEdit->text().isEmpty() );
        break;
    case MemberType_t::Differentiation:
        result = ! ( this->m_ui->kLineEdit->text().isEmpty()  ||
                     this->m_ui->maxTLineEdit->text().isEmpty() );
        break;

    default:
        break;
    }

    return result;
}

bool ThemeWidget::isAllDataProvided_PID()
{
    return !( this->m_ui->kpLineEdit->text().isEmpty()    ||
              this->m_ui->kiLineEdit->text().isEmpty()    ||
              this->m_ui->kdLineEdit->text().isEmpty()    ||
              this->m_ui->dtLineEdit->text().isEmpty()    ||
              this->m_ui->startLineEdit->text().isEmpty() ||
              this->m_ui->targetLineEdit->text().isEmpty()||
              this->m_ui->maxTLineEdit->text().isEmpty()  );
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

    connect(m_ui->tDlineEdit,
            &QLineEdit::textChanged,
            this,
            &ThemeWidget::enableShowGraphButton);


    connect(m_ui->kpLineEdit,
            &QLineEdit::textChanged,
            this,
            &ThemeWidget::enableShowGraphButton);

    connect(m_ui->kiLineEdit,
            &QLineEdit::textChanged,
            this,
            &ThemeWidget::enableShowGraphButton);

    connect(m_ui->kdLineEdit,
            &QLineEdit::textChanged,
            this,
            &ThemeWidget::enableShowGraphButton);

    connect(m_ui->dtLineEdit,
            &QLineEdit::textChanged,
            this,
            &ThemeWidget::enableShowGraphButton);

    connect(m_ui->targetLineEdit,
            &QLineEdit::textChanged,
            this,
            &ThemeWidget::enableShowGraphButton);

    connect(m_ui->startLineEdit,
            &QLineEdit::textChanged,
            this,
            &ThemeWidget::enableShowGraphButton);


    connect(m_ui->memberTypeComboBox,       //FIXME will it work with ,,||" ?
            &QComboBox::currentTextChanged,
            this,
            &ThemeWidget::enableShowGraphButton);
}

void ThemeWidget::responseChangedCallback(int index)
{
    this->_whichResponseIsPicked = static_cast<Calculation::ResponseType_t>(index+1);
}

void ThemeWidget::characteristicChangedCallback(int index)
{
    this->_whichCharactersiticIsPicked = static_cast<Calculation::CharacteristicType_t>(index+1);

    switch(_whichCharactersiticIsPicked)
    {
    case CharacteristicType_t::Time:
        m_ui->memberTypeComboBox->setEnabled(true);
        m_ui->signalTypeComboBox->setEnabled(true);
        this->main_chart->axes(Qt::Horizontal).first()->setTitleText(tr("t[s]"));
        this->main_chart->axes(Qt::Vertical).first()->setTitleText(tr("h(t)"));
        m_ui->maxTLineEdit->setText("100");
        break;

    case CharacteristicType_t::AmplitudePhase:
        m_ui->memberTypeComboBox->setEnabled(true);
        m_ui->signalTypeComboBox->setEnabled(false);
        this->main_chart->axes(Qt::Horizontal).first()->setTitleText(tr("Re"));
        this->main_chart->axes(Qt::Vertical).first()->setTitleText(tr("Im"));
        break;

    case CharacteristicType_t::PID:
        m_ui->memberTypeComboBox->setEnabled(false);
        m_ui->signalTypeComboBox->setEnabled(false);
        this->main_chart->axes(Qt::Horizontal).first()->setTitleText(tr("t[s]"));
        this->main_chart->axes(Qt::Vertical).first()->setTitleText(tr("h(t)"));
        m_ui->maxTLineEdit->setText("100");
        break;

    default:
        break;
    }

    this->setVisibilityOfWidgetFields(_whichCharactersiticIsPicked);

    return;
}

void ThemeWidget::setVisibilityOfWidgetFields(CharacteristicType_t characteristicType)
{
    switch(characteristicType)
    {
    case CharacteristicType_t::Time:
        m_ui->kLineEdit->setVisible(true);
        m_ui->t1LineEdit->setVisible(true);
        m_ui->t2LineEdit->setVisible(true);
        m_ui->t3LineEdit->setVisible(true);
        m_ui->t4LineEdit->setVisible(true);
        m_ui->tDlineEdit->setVisible(true);

        m_ui->kpLineEdit->setVisible(false);
        m_ui->kiLineEdit->setVisible(false);
        m_ui->kdLineEdit->setVisible(false);
        m_ui->dtLineEdit->setVisible(false);
        m_ui->targetLineEdit->setVisible(false);
        m_ui->startLineEdit->setVisible(false);

        m_ui->kLabel->setVisible(true);
        m_ui->t1Label->setVisible(true);
        m_ui->t2Label->setVisible(true);
        m_ui->t3Label->setVisible(true);
        m_ui->t4Label->setVisible(true);
        m_ui->tDlabel ->setVisible(true);

        m_ui->kpLabel->setVisible(false);
        m_ui->kiLabel->setVisible(false);
        m_ui->kdLabel->setVisible(false);
        m_ui->dtLabel->setVisible(false);
        m_ui->targetLabel->setVisible(false);
        m_ui->startLabel->setVisible(false);

        m_ui->maxTLineEdit->setVisible(true);
        m_ui->maxTLabel->setVisible(true);
        break;

    case CharacteristicType_t::AmplitudePhase:
        m_ui->kLineEdit->setVisible(true);
        m_ui->t1LineEdit->setVisible(true);
        m_ui->t2LineEdit->setVisible(true);
        m_ui->t3LineEdit->setVisible(true);
        m_ui->t4LineEdit->setVisible(true);
        m_ui->tDlineEdit->setVisible(true);

        m_ui->kpLineEdit->setVisible(false);
        m_ui->kiLineEdit->setVisible(false);
        m_ui->kdLineEdit->setVisible(false);
        m_ui->dtLineEdit->setVisible(false);
        m_ui->targetLineEdit->setVisible(false);
        m_ui->startLineEdit->setVisible(false);

        m_ui->kLabel->setVisible(true);
        m_ui->t1Label->setVisible(true);
        m_ui->t2Label->setVisible(true);
        m_ui->t3Label->setVisible(true);
        m_ui->t4Label->setVisible(true);
        m_ui->tDlabel ->setVisible(true);

        m_ui->kpLabel->setVisible(false);
        m_ui->kiLabel->setVisible(false);
        m_ui->kdLabel->setVisible(false);
        m_ui->dtLabel->setVisible(false);
        m_ui->targetLabel->setVisible(false);
        m_ui->startLabel->setVisible(false);

        m_ui->maxTLineEdit->setVisible(false);
        m_ui->maxTLabel->setVisible(false);
        break;

    case CharacteristicType_t::PID:
        m_ui->kLineEdit->setVisible(false);
        m_ui->t1LineEdit->setVisible(false);
        m_ui->t2LineEdit->setVisible(false);
        m_ui->t3LineEdit->setVisible(false);
        m_ui->t4LineEdit->setVisible(false);
        m_ui->tDlineEdit->setVisible(false);

        m_ui->kpLineEdit->setVisible(true);
        m_ui->kiLineEdit->setVisible(true);
        m_ui->kdLineEdit->setVisible(true);
        m_ui->dtLineEdit->setVisible(true);
        m_ui->targetLineEdit->setVisible(true);
        m_ui->startLineEdit->setVisible(true);

        m_ui->kLabel->setVisible(false);
        m_ui->t1Label->setVisible(false);
        m_ui->t2Label->setVisible(false);
        m_ui->t3Label->setVisible(false);
        m_ui->t4Label->setVisible(false);
        m_ui->tDlabel ->setVisible(false);

        m_ui->kpLabel->setVisible(true);
        m_ui->kiLabel->setVisible(true);
        m_ui->kdLabel->setVisible(true);
        m_ui->dtLabel->setVisible(true);
        m_ui->targetLabel->setVisible(true);
        m_ui->startLabel->setVisible(true);

        m_ui->maxTLineEdit->setVisible(true);
        m_ui->maxTLabel->setVisible(true);
        break;

    default:
        break;
    }

    return;
}

void ThemeWidget::memberChangedCallback(int index)
{
    this->_whichMemberIsPicked = static_cast<MemberType_t>(index+1);

    switch(this->_whichMemberIsPicked)
    {
    case MemberType_t::Proportional:
    case MemberType_t::Differentiation:
        m_ui->t1LineEdit->setEnabled(false);
        m_ui->t2LineEdit->setEnabled(false);
        m_ui->t3LineEdit->setEnabled(false);
        m_ui->t4LineEdit->setEnabled(false);
        m_ui->tDlineEdit->setEnabled(false);

        m_ui->t1LineEdit->clear();
        m_ui->t2LineEdit->clear();
        m_ui->t3LineEdit->clear();
        m_ui->t4LineEdit->clear();
        m_ui->tDlineEdit->clear();
        break;

    case MemberType_t::InertionFirstOrder:
        m_ui->t1LineEdit->setEnabled(true);
        m_ui->t2LineEdit->setEnabled(false);
        m_ui->t3LineEdit->setEnabled(false);
        m_ui->t4LineEdit->setEnabled(false);
        m_ui->tDlineEdit->setEnabled(false);

        m_ui->t2LineEdit->clear();
        m_ui->t3LineEdit->clear();
        m_ui->t4LineEdit->clear();
        m_ui->tDlineEdit->clear();
        break;

    case MemberType_t::InertionSecondOrder:
        m_ui->t1LineEdit->setEnabled(true);
        m_ui->t2LineEdit->setEnabled(true);
        m_ui->t3LineEdit->setEnabled(false);
        m_ui->t4LineEdit->setEnabled(false);
        m_ui->tDlineEdit->setEnabled(false);

        m_ui->t3LineEdit->clear();
        m_ui->t4LineEdit->clear();
        m_ui->tDlineEdit->clear();
        break;

    case MemberType_t::InertionThirdOrder:
        m_ui->t1LineEdit->setEnabled(true);
        m_ui->t2LineEdit->setEnabled(true);
        m_ui->t3LineEdit->setEnabled(true);
        m_ui->t4LineEdit->setEnabled(false);
        m_ui->tDlineEdit->setEnabled(false);

        m_ui->t4LineEdit->clear();
        m_ui->tDlineEdit->clear();
        break;

    case MemberType_t::InertionFourthOrder:
        m_ui->t1LineEdit->setEnabled(true);
        m_ui->t2LineEdit->setEnabled(true);
        m_ui->t3LineEdit->setEnabled(true);
        m_ui->t4LineEdit->setEnabled(true);
        m_ui->tDlineEdit->setEnabled(false);

        m_ui->tDlineEdit->clear();
        break;

    case MemberType_t::Integration:
        m_ui->t1LineEdit->setEnabled(false);
        m_ui->t2LineEdit->setEnabled(false);
        m_ui->t3LineEdit->setEnabled(false);
        m_ui->t4LineEdit->setEnabled(false);
        m_ui->tDlineEdit->setEnabled(true);

        m_ui->t1LineEdit->clear();
        m_ui->t2LineEdit->clear();
        m_ui->t3LineEdit->clear();
        m_ui->t4LineEdit->clear();
        break;

    default:
        break;
    }

}

//TODO this function should be moved to calculation.cpp
DataTable ThemeWidget::calculate(Calculation::DataAcquired_t& data)
{
    DataTable result;

    data.memberType = this->_whichMemberIsPicked;
    data.responseType = this->_whichResponseIsPicked;
    data.characteristicType = this->_whichCharactersiticIsPicked;
//    maxTime = m_ui->line
    QPair<int, int> span(0, _data.maxT);
    result = this->_calculator.calculate(data, span);   //TODO Maybe make it rather a static class?

    return result;
}
