#include "MyForm.h"
#include "math.h"

using namespace Lab2;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

using namespace System::Windows::Forms::DataVisualization::Charting;
[STAThread]
void Main(array<String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	MyForm form;
	Application::Run(%form);
}

System::Void Lab2::MyForm::button1_Click(System::Object^  sender, System::EventArgs^  e)
{
	button1->Enabled = false;
	// p = 3; //roky   10-2 = 8
	// alpha = 0.8; //nadijnist
	//tablycja stjudenta ======  t (9,0.8) = 1.3830     t(8,0.8)=1.3968
	//zatraty na zberigannja, tys. grn
	double t = 1.3830;
	int N = 13, n = 10;
	double *zatraty = new double[N];  double *years = new double[N];
	
	double kk[2][10] = { { 4.5, 3.95, 3.2, 2.7, 2.55, 2.3, 2.3, 2.05, 2.1, 2.05 },
					{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 } };
	
	
	for (unsigned int i = 0; i < 10; i++)
	{
		zatraty[i] = kk[0][i];   //y
		years[i] = kk[1][i];     // x
	}
	
	
	Series^  series1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
	series1->ChartArea = L"ChartArea1";
	series1->ChartType = SeriesChartType::Point;
	series1->Name = L"Series1";
	chart1->Series->Add(series1);
	for (int i = 0; i < 10; i++) series1->Points->AddXY(years[i], zatraty[i]);

	Series^  series5 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
	series5->ChartArea = L"ChartArea1";
	series5->ChartType = SeriesChartType::Spline;
	series5->Name = L"Series5";
	chart1->Series->Add(series5);
	for (int i = 0; i < 10; i++) series5->Points->AddXY(years[i], zatraty[i]);

	//-----------------------------------------------------
	for (int i = 0; i < 10; i++) kk[1][i] = 1 / kk[1][i]; //arrayof x
	
	double sumxi=0, sumxi2 = 0, sumyi = 0, sumxiyi = 0;
	for (unsigned int i = 0; i < 10; i++)
	{
		sumxi += kk[1][i];
		sumxi2 += kk[1][i] * kk[1][i];
		sumyi += kk[0][i];
		sumxiyi += kk[1][i] * kk[0][i];
	}

	//zamina 1/t = x=======> y = a + b/t======> y = a + bx
	//for (int i = 0; i < 10; i++) kk[1][i] -= 2007;

	double a, b;
	a = (sumyi * sumxi2 - sumxi * sumxiyi) / (n*sumxi2 - sumxi * sumxi);
	b = (n* sumxiyi - sumxi * sumyi) / (n*sumxi2 - sumxi * sumxi);
	textBox1->Text += " a = " + a + ", b = " + b;

	//-----linija regresiji-------
	Series^  series2 = gcnew Series();
	series2->ChartArea = L"ChartArea1";
	series2->ChartType = SeriesChartType::Point;
	series2->Name = L"Series2";
	chart1->Series->Add(series2);
		
	for (unsigned int i = 0; i < N; i++)
	{
		years[i] = i+1;     // x
		//regry[i] = a * pow(b,-regrx[i]) + 3;   //y
		zatraty[i] = a +b/years[i];   //y
		series2->Points->AddXY(years[i], zatraty[i]);
	}

	Series^  series6 = gcnew Series();
	series6->ChartArea = L"ChartArea1";
	series6->ChartType = SeriesChartType::Spline;
	series6->Name = L"Series6";
	chart1->Series->Add(series6);
	for (unsigned int i = 0; i < N; i++) series6->Points->AddXY(years[i], zatraty[i]);

	//------------
	double deltayi[13], sigmax = 0;
	for (unsigned int i = 0; i < n; i++) sigmax += (kk[1][i] - sumxi/10)*(kk[1][i] - sumxi/10);
	sigmax = sqrt(sigmax/n);
	
	double sigma = 0;
	for (unsigned int i = 0; i < n; i++) sigma += (kk[1][i] - zatraty[i]/10)*(kk[1][i] - zatraty[i]/10);
	sigma = sqrt(sigma/8);

	textBox1->Text += "\r\n sigmax " + sigmax + "\r\n sigma " + sigma;

	for (unsigned int i = 0; i < N; i++)
	{
		if(i >= 10) deltayi[i] = t * sigma*sqrt(n+1 + (years[i] - sumxi/n)*(years[i] -sumxi/n) / (sigmax*sigmax)) / sqrt(n); //prognoz
		else deltayi[i] = t * sigma* sqrt(1 + (years[i] - sumxi/n) * (years[i] - sumxi/n) / (sigmax*sigmax)) / sqrt(n);
		textBox1->Text += "\r\n deltayi " + deltayi[i];
	}

	Series^  series3 = gcnew Series();
	series3->ChartArea = L"ChartArea1";
	series3->ChartType = SeriesChartType::Point;
	series3->Name = L"Series3";
	chart1->Series->Add(series3);
	for (unsigned int i = 0; i < N; i++) series3->Points->AddXY(years[i], zatraty[i]+ deltayi[i]);

	Series^  series4 = gcnew Series();
	series4->ChartArea = L"ChartArea1";
	series4->ChartType = SeriesChartType::Point;
	series4->Name = L"Series4";
	chart1->Series->Add(series4);
	for (unsigned int i = 0; i < N; i++) series4->Points->AddXY(years[i], zatraty[i] - deltayi[i]);
	//----------------------------------------	
	Series^  series7 = gcnew Series();
	series7->ChartArea = L"ChartArea1";
	series7->ChartType = SeriesChartType::Spline;
	series7->Name = L"Series7";
	chart1->Series->Add(series7);
	for (unsigned int i = 0; i < N; i++) series7->Points->AddXY(years[i], zatraty[i] + deltayi[i]);

	Series^  series8 = gcnew Series();
	series8->ChartArea = L"ChartArea1";
	series8->ChartType = SeriesChartType::Spline;
	series8->Name = L"Series8";
	chart1->Series->Add(series8);
	for (unsigned int i = 0; i < N; i++) series8->Points->AddXY(years[i], zatraty[i] - deltayi[i]);

	
	//---------------tisnota zvjazkiv-----
	double sigmay2 = 0;
	for (unsigned int i = 0; i < N; i++) sigmay2 += (kk[0][i] - sumyi / 10)*(kk[0][i] - sumyi/ 10);
	sigmay2 /= n;
	double R = sqrt(1-(sigma / (8*sigmay2)));
	textBox1->Text += "\r\n R = " + R;

	delete[]years;
	delete[]zatraty;
}
