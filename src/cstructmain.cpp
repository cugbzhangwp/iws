#include "band_passstruct.h"
#include "fir_list.h"
#include <stdio.h>
//#include <iostream>
#include "math.h"
using namespace std;
void main()
	{
	float data[1000];
	double dataOut;
	double pi=3.14159265358979323;
	int sample_rate=200; //  改变采样率
	for(int i=0;i<1000;i++)
		{
		//data[i]=/*pow(-1.0,double(i))**/i;
		data[i]=sin(2*pi/16*i);
		}
	FIR_LIST ssdata[1000];
	ssdata[0].data=data[0];
	ssdata[0].next=&ssdata[1];
	ssdata[0].sample_rata=sample_rate;

	 for(int i=1;i<999;i++)
		{
		ssdata[i].data=data[i];
		ssdata[i].last=&ssdata[i-1];
		ssdata[i].next=&ssdata[i+1];
		ssdata[i].sample_rata=sample_rate;
		}

	 ssdata[999].data=data[999];
	 ssdata[999].last=&ssdata[998];
	 ssdata[999].sample_rata=sample_rate;
	 if(sample_rate==50)
		 {

		 
		for(int i=200;i<400;i++)
			{

			band_passstruct(&ssdata[i],dataOut);
			//cout<<dataOut<<"    "<<data[80]<<endl;
			printf("%16.8f  %f \n",dataOut,data[i-40] );
			}
	     }

	 if(sample_rate==100)
		 {


		 for(int i=200;i<400;i++)
			 {

			 band_passstruct(&ssdata[i],dataOut);
			 //cout<<dataOut<<"    "<<data[80]<<endl;
			 printf("%16.8f  %f \n",dataOut,data[i-95] );
			 }
		 }

	 if(sample_rate==200)
		 {


		 for(int i=400;i<600;i++)
			 {

			 band_passstruct(&ssdata[i],dataOut);
			 //cout<<dataOut<<"    "<<data[80]<<endl;
			 printf("%16.8f  %f \n",dataOut,data[i-150] );
			 }
		 }





	//band_pass(data,191,200,dataOut);
	////cout<<dataOut<<"    "<<data[60]<<endl;
	//printf("%d   %d \n",dataOut,data[190-95] );
	//band_pass(data,301,400,dataOut);
	////cout<<dataOut<<"    "<<data[40]<<endl;
	//printf("%d   %d \n",dataOut,data[300-150] );
	//cout<<"press enter kye to continue......"<<endl;
	//cin.get();
	getchar();
	}