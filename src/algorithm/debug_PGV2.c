int PGA_V_G2(FIR_LIST *i_fir_list,BAND_PASS_RESULT *band_pass_data,int *count,int timespan,int *trig,int PGV_type,int band_pass_flag)
	{
		printf("band_pass_flag=%d\n",band_pass_flag);
		//exit(0);
		//return 0;
		alg_locat();
	int firLength,j,jjj;
	double datamid;
	FIR_LIST *now_fir;
	if (i_fir_list->sample_rata==50&&band_pass_flag==0)
		{
		firLength=81;
		double Fir[81]={-4.7987787339820936e-04,
			-8.9057350076593368e-04,
			-7.2903341056485352e-04,
			-9.3968046651590189e-04,
			-2.0015152670250906e-03,
			-1.8457782153191327e-03,
			-1.4331943313841396e-03,
			-3.2987790655067970e-03,
			-3.9188767708761094e-03,
			-2.2358326021832526e-03,
			-4.2761095213556272e-03,
			-6.9685597648698229e-03,
			-4.0562398557881119e-03,
			-4.5086713435378723e-03,
			-1.0324010165847381e-02,
			-7.7980789590883538e-03,
			-4.2123548102063557e-03,
			-1.2580685653923828e-02,
			-1.3896183105007932e-02,
			-4.6707793854684938e-03,
			-1.2139776287417038e-02,
			-2.1492365469531257e-02,
			-8.0968892823001046e-03,
			-8.2631555848332473e-03,
			-2.7966602054323594e-02,
			-1.6735759731169867e-02,
			-2.1834926757662077e-03,
			-2.9254096192636532e-02,
			-3.1499163705017090e-02,
			2.3107659525776346e-03,
			-2.0832464828927390e-02,
			-5.0853889965031082e-02,
			-1.1624388651572256e-03,
			1.8841462457617236e-03,
			-7.0730297852423088e-02,
			-2.3669862992490260e-02,
			4.9655253129437991e-02,
			-8.5754335252881606e-02,
			-1.1745446174509039e-01,
			2.7761355362248835e-01,
			5.7538029520670841e-01,
			2.7761355362248769e-01,
			-1.1745446174508975e-01,
			-8.5754335252880912e-02,
			4.9655253129437457e-02,
			-2.3669862992489934e-02,
			-7.0730297852421936e-02,
			1.8841462457616889e-03,
			-1.1624388651572048e-03,
			-5.0853889965029819e-02,
			-2.0832464828926815e-02,
			2.3107659525775617e-03,
			-3.1499163705016028e-02,
			-2.9254096192635470e-02,
			-2.1834926757661245e-03,
			-1.6735759731169159e-02,
			-2.7966602054322321e-02,
			-8.2631555848328413e-03,
			-8.0968892822996814e-03,
			-2.1492365469530057e-02,
			-1.2139776287416327e-02,
			-4.6707793854682024e-03,
			-1.3896183105007011e-02,
			-1.2580685653922945e-02,
			-4.2123548102060435e-03,
			-7.7980789590877501e-03,
			-1.0324010165846531e-02,
			-4.5086713435374846e-03,
			-4.0562398557877433e-03,
			-6.9685597648691568e-03,
			-4.2761095213552005e-03,
			-2.2358326021830158e-03,
			-3.9188767708756775e-03,
			-3.2987790655064630e-03,
			-1.4331943313839600e-03,
			-1.8457782153188935e-03,
			-2.0015152670248100e-03,
			-9.3968046651576485e-04,
			-7.2903341056474108e-04,
			-8.9057350076577387e-04,
			-4.7987787339812745e-04,
			};
		if(*trig!=0)
			{
			if(PGV_type==1)
				{
				datamid=0.0;
				now_fir=i_fir_list;
				for(int i=0;i<firLength;i++)
					{
					datamid=datamid+now_fir->dataV[4]*Fir[i];
					now_fir=now_fir->last;
					}
				i_fir_list->band_pass_dataV[4]=datamid;



				datamid=0.0;
				now_fir=i_fir_list;
				for(int i=0;i<firLength;i++)
					{
					datamid=datamid+now_fir->dataD[4]*Fir[i];
					now_fir=now_fir->last;
					}
				i_fir_list->band_pass_dataD[4]=datamid;

				}
			else if(PGV_type==2)
				{
				for(int j=0;j<3;j++)
					{


					datamid=0.0;
					now_fir=i_fir_list;
					for(int i=0;i<firLength;i++)
						{
						datamid=datamid+now_fir->dataV[j]*Fir[i];
						now_fir=now_fir->last;
						}
					i_fir_list->band_pass_dataV[j]=datamid;

					}
				i_fir_list->band_pass_dataV[4]=pow((double)(i_fir_list->band_pass_dataV[0]*i_fir_list->band_pass_dataV[0]+i_fir_list->band_pass_dataV[1]*i_fir_list->band_pass_dataV[1]),0.5);
				for(int j=0;j<3;j++)
					{


					datamid=0.0;
					now_fir=i_fir_list;
					for(int i=0;i<firLength;i++)
						{
						datamid=datamid+now_fir->dataD[j]*Fir[i];
						now_fir=now_fir->last;
						}
					i_fir_list->band_pass_dataD[j]=datamid;
					}
				i_fir_list->band_pass_dataD[4]=pow((double)(i_fir_list->band_pass_dataD[0]*i_fir_list->band_pass_dataD[0]+i_fir_list->band_pass_dataD[1]*i_fir_list->band_pass_dataD[1]),0.5);
				}




			if(*trig==1)
				{
				for(jjj=0;jjj<3;jjj++)
					{
					band_pass_data->final_PGA_component[jjj]=abs(i_fir_list->band_pass_data[jjj]);
					band_pass_data->final_PGV_component[jjj]=abs(i_fir_list->band_pass_dataV[jjj]);
					band_pass_data->final_PGD_component[jjj]=abs(i_fir_list->band_pass_dataD[jjj]);
					}
				band_pass_data->final_PGA=i_fir_list->band_pass_data[4];
				band_pass_data->final_PGV=i_fir_list->band_pass_dataV[4];
				band_pass_data->final_PGD=i_fir_list->band_pass_dataD[4];
				*count+=1;
				*trig=2;
				}
			else
				{
				for(jjj=0;jjj<3;jjj++)
					{
					if(abs(i_fir_list->band_pass_data[jjj])>band_pass_data->final_PGA_component[jjj])
						band_pass_data->final_PGA_component[jjj]=abs(i_fir_list->band_pass_data[jjj]);
					if(abs(i_fir_list->band_pass_dataV[jjj])>band_pass_data->final_PGV_component[jjj])
						band_pass_data->final_PGV_component[jjj]=abs(i_fir_list->band_pass_dataV[jjj]);
					if(abs(i_fir_list->band_pass_dataD[jjj])>band_pass_data->final_PGD_component[jjj])
						band_pass_data->final_PGD_component[jjj]=abs(i_fir_list->band_pass_dataD[jjj]);
					}




				if(i_fir_list->band_pass_data[4]>band_pass_data->final_PGA)
					{
					band_pass_data->final_PGA=i_fir_list->band_pass_data[4];
					band_pass_data->utc_second_final_PGA=i_fir_list->utc_second; //utc时间秒
					band_pass_data->utc_nanosecond_final_PGA=i_fir_list->utc_nanosecond; //utc时间纳秒

					}
				//else
				//	{
				//	band_pass_data->final_PGA=i_fir_list->last->final_PGA;
				//	band_pass_data->utc_second_final_PGA=i_fir_list->last->utc_second; //utc时间秒
				//	band_pass_data->utc_nanosecond_final_PGA=i_fir_list->last->utc_nanosecond; //utc时间纳秒
				//	}
				if(i_fir_list->band_pass_dataV[4]>band_pass_data->final_PGV)
					{
					band_pass_data->final_PGV=i_fir_list->band_pass_dataV[4];
					band_pass_data->utc_second_final_PGV=i_fir_list->utc_second; //utc时间秒
					band_pass_data->utc_nanosecond_final_PGV=i_fir_list->utc_nanosecond; //utc时间纳秒
					}
				//else
				//	{
				//	band_pass_data->final_PGV=i_fir_list->last->final_PGV;
				//	band_pass_data->utc_second_final_PGV=i_fir_list->last->utc_second; //utc时间秒
				//	band_pass_data->utc_nanosecond_final_PGV=i_fir_list->last->utc_nanosecond; //utc时间纳秒
				//	}
				if(i_fir_list->band_pass_dataD[4]>band_pass_data->final_PGD)
					{
					band_pass_data->final_PGD=i_fir_list->band_pass_dataD[4];
					band_pass_data->utc_second_final_PGD=i_fir_list->utc_second; //utc时间秒
					band_pass_data->utc_nanosecond_final_PGD=i_fir_list->utc_nanosecond; //utc时间纳秒
					}
				//else
				//	{
				//	band_pass_data->final_PGD=i_fir_list->last->final_PGD;
				//	band_pass_data->utc_second_final_PGD=i_fir_list->last->utc_second; //utc时间秒
				//	band_pass_data->utc_nanosecond_final_PGD=i_fir_list->last->utc_nanosecond; //utc时间纳秒
				//	}
				if(*count==timespan*50)

					{
					*count=1;
					return 2;
					}
				else
					{


					*count+=1;
					}



				}
			}
		return 1;
		}
	else if (i_fir_list->sample_rata==50&&band_pass_flag==1)
			{
			firLength=151;
			double Fir[151]={-2.8052426906829513e-03,
				-4.2393366582970269e-03,
				-3.8887138369385330e-03,
				-2.2354116003412555e-03,
				-1.7151340451976370e-03,
				-3.3681835107814504e-03,
				-5.1779067586189562e-03,
				-4.6965135173902524e-03,
				-2.5638580821051682e-03,
				-1.8818136148366425e-03,
				-3.9473893023176630e-03,
				-6.1968403681345145e-03,
				-5.5591924712764754e-03,
				-2.8602285633303683e-03,
				-1.9855715179514558e-03,
				-4.5343773820232715e-03,
				-7.2993746523036347e-03,
				-6.4757972766725895e-03,
				-3.1075377181290190e-03,
				-2.0034515060087738e-03,
				-5.1201983246674521e-03,
				-8.4924282172401180e-03,
				-7.4477448916228876e-03,
				-3.2854307785719003e-03,
				-1.9068638995382889e-03,
				-5.6956197214221181e-03,
				-9.7890130188554095e-03,
				-8.4805326328304762e-03,
				-3.3686880184671422e-03,
				-1.6587501518070553e-03,
				-6.2513201008398845e-03,
				-1.1212180130190147e-02,
				-9.5865220664507079e-03,
				-3.3245287368907092e-03,
				-1.2085331900947341e-03,
				-6.7780883098406538e-03,
				-1.2801858768458711e-02,
				-1.0789770869833983e-02,
				-3.1075700373275851e-03,
				-4.8269424897207818e-04,
				-7.2670234585519751e-03,
				-1.4627552535563389e-02,
				-1.2135072981765502e-02,
				-2.6498708200075548e-03,
				6.3396297658225372e-04,
				-7.7097304696529074e-03,
				-1.6813951744110978e-02,
				-1.3706460562373948e-02,
				-1.8396919533613628e-03,
				2.3388911427624061e-03,
				-8.0985063426770101e-03,
				-1.9598293978969096e-02,
				-1.5669566680424962e-02,
				-4.7103250693004398e-04,
				5.0106092712312988e-03,
				-8.4265124453908481e-03,
				-2.3477962536707364e-02,
				-1.8384256507005932e-02,
				1.8961989182070325e-03,
				9.4930912865623281e-03,
				-8.6879284730553841e-03,
				-2.9673940083844816e-02,
				-2.2776743005803374e-02,
				6.4259738795488016e-03,
				1.8156146307088068e-02,
				-8.8780841635862553e-03,
				-4.2138499851260856e-02,
				-3.2106505797306333e-02,
				1.7595589150292312e-02,
				4.1258267838167328e-02,
				-8.9935654105577235e-03,
				-8.4619383336774115e-02,
				-7.1400968296348372e-02,
				8.4628282316732584e-02,
				2.9419739638965176e-01,
				3.9169381130492797e-01,
				2.9419739638965176e-01,
				8.4628282316732584e-02,
				-7.1400968296348372e-02,
				-8.4619383336774115e-02,
				-8.9935654105577252e-03,
				4.1258267838167349e-02,
				1.7595589150292319e-02,
				-3.2106505797306346e-02,
				-4.2138499851260863e-02,
				-8.8780841635862588e-03,
				1.8156146307088072e-02,
				6.4259738795488016e-03,
				-2.2776743005803384e-02,
				-2.9673940083844830e-02,
				-8.6879284730553893e-03,
				9.4930912865623333e-03,
				1.8961989182070343e-03,
				-1.8384256507005946e-02,
				-2.3477962536707378e-02,
				-8.4265124453908533e-03,
				5.0106092712313005e-03,
				-4.7103250693004398e-04,
				-1.5669566680424969e-02,
				-1.9598293978969110e-02,
				-8.0985063426770188e-03,
				2.3388911427624079e-03,
				-1.8396919533613663e-03,
				-1.3706460562373958e-02,
				-1.6813951744110992e-02,
				-7.7097304696529126e-03,
				6.3396297658225372e-04,
				-2.6498708200075574e-03,
				-1.2135072981765515e-02,
				-1.4627552535563406e-02,
				-7.2670234585519829e-03,
				-4.8269424897207731e-04,
				-3.1075700373275877e-03,
				-1.0789770869833994e-02,
				-1.2801858768458723e-02,
				-6.7780883098406616e-03,
				-1.2085331900947350e-03,
				-3.3245287368907140e-03,
				-9.5865220664507183e-03,
				-1.1212180130190158e-02,
				-6.2513201008398932e-03,
				-1.6587501518070587e-03,
				-3.3686880184671461e-03,
				-8.4805326328304884e-03,
				-9.7890130188554234e-03,
				-5.6956197214221250e-03,
				-1.9068638995382910e-03,
				-3.2854307785719064e-03,
				-7.4477448916228997e-03,
				-8.4924282172401319e-03,
				-5.1201983246674599e-03,
				-2.0034515060087777e-03,
				-3.1075377181290238e-03,
				-6.4757972766725999e-03,
				-7.2993746523036451e-03,
				-4.5343773820232776e-03,
				-1.9855715179514588e-03,
				-2.8602285633303726e-03,
				-5.5591924712764867e-03,
				-6.1968403681345232e-03,
				-3.9473893023176700e-03,
				-1.8818136148366458e-03,
				-2.5638580821051716e-03,
				-4.6965135173902611e-03,
				-5.1779067586189640e-03,
				-3.3681835107814543e-03,
				-1.7151340451976407e-03,
				-2.2354116003412351e-03,
				-3.8887138369385656e-03,
				-4.2393366582969870e-03,
				-2.8052426906829439e-03,
				};
			if(*trig!=0)
				{
				if(PGV_type==1)
					{
					datamid=0.0;
					now_fir=i_fir_list;
					for(int i=0;i<firLength;i++)
						{
						datamid=datamid+now_fir->dataV[4]*Fir[i];
						now_fir=now_fir->last;
						}
					i_fir_list->band_pass_dataV[4]=datamid;



					datamid=0.0;
					now_fir=i_fir_list;
					for(int i=0;i<firLength;i++)
						{
						datamid=datamid+now_fir->dataD[4]*Fir[i];
						now_fir=now_fir->last;
						}
					i_fir_list->band_pass_dataD[4]=datamid;

					}
				else if(PGV_type==2)
					{
					for(int j=0;j<3;j++)
						{


						datamid=0.0;
						now_fir=i_fir_list;
						for(int i=0;i<firLength;i++)
							{
							datamid=datamid+now_fir->dataV[j]*Fir[i];
							now_fir=now_fir->last;
							}
						i_fir_list->band_pass_dataV[j]=datamid;

						}
					i_fir_list->band_pass_dataV[4]=pow((double)(i_fir_list->band_pass_dataV[0]*i_fir_list->band_pass_dataV[0]+i_fir_list->band_pass_dataV[1]*i_fir_list->band_pass_dataV[1]),0.5);
					for(int j=0;j<3;j++)
						{


						datamid=0.0;
						now_fir=i_fir_list;
						for(int i=0;i<firLength;i++)
							{
							datamid=datamid+now_fir->dataD[j]*Fir[i];
							now_fir=now_fir->last;
							}
						i_fir_list->band_pass_dataD[j]=datamid;
						}
					i_fir_list->band_pass_dataD[4]=pow((double)(i_fir_list->band_pass_dataD[0]*i_fir_list->band_pass_dataD[0]+i_fir_list->band_pass_dataD[1]*i_fir_list->band_pass_dataD[1]),0.5);
					}




				if(*trig==1)
					{
					for(jjj=0;jjj<3;jjj++)
						{
						band_pass_data->final_PGA_component[jjj]=abs(i_fir_list->band_pass_data[jjj]);
						band_pass_data->final_PGV_component[jjj]=abs(i_fir_list->band_pass_dataV[jjj]);
						band_pass_data->final_PGD_component[jjj]=abs(i_fir_list->band_pass_dataD[jjj]);
						}
					band_pass_data->final_PGA=i_fir_list->band_pass_data[4];
					band_pass_data->final_PGV=i_fir_list->band_pass_dataV[4];
					band_pass_data->final_PGD=i_fir_list->band_pass_dataD[4];
					*count+=1;
					*trig=2;
					}
				else
					{
					for(jjj=0;jjj<3;jjj++)
						{
						if(abs(i_fir_list->band_pass_data[jjj])>band_pass_data->final_PGA_component[jjj])
							band_pass_data->final_PGA_component[jjj]=abs(i_fir_list->band_pass_data[jjj]);
						if(abs(i_fir_list->band_pass_dataV[jjj])>band_pass_data->final_PGV_component[jjj])
							band_pass_data->final_PGV_component[jjj]=abs(i_fir_list->band_pass_dataV[jjj]);
						if(abs(i_fir_list->band_pass_dataD[jjj])>band_pass_data->final_PGD_component[jjj])
							band_pass_data->final_PGD_component[jjj]=abs(i_fir_list->band_pass_dataD[jjj]);
						}




					if(i_fir_list->band_pass_data[4]>band_pass_data->final_PGA)
						{
						band_pass_data->final_PGA=i_fir_list->band_pass_data[4];
						band_pass_data->utc_second_final_PGA=i_fir_list->utc_second; //utc时间秒
						band_pass_data->utc_nanosecond_final_PGA=i_fir_list->utc_nanosecond; //utc时间纳秒

						}
					//else
					//	{
					//	band_pass_data->final_PGA=i_fir_list->last->final_PGA;
					//	band_pass_data->utc_second_final_PGA=i_fir_list->last->utc_second; //utc时间秒
					//	band_pass_data->utc_nanosecond_final_PGA=i_fir_list->last->utc_nanosecond; //utc时间纳秒
					//	}
					if(i_fir_list->band_pass_dataV[4]>band_pass_data->final_PGV)
						{
						band_pass_data->final_PGV=i_fir_list->band_pass_dataV[4];
						band_pass_data->utc_second_final_PGV=i_fir_list->utc_second; //utc时间秒
						band_pass_data->utc_nanosecond_final_PGV=i_fir_list->utc_nanosecond; //utc时间纳秒
						}
					//else
					//	{
					//	band_pass_data->final_PGV=i_fir_list->last->final_PGV;
					//	band_pass_data->utc_second_final_PGV=i_fir_list->last->utc_second; //utc时间秒
					//	band_pass_data->utc_nanosecond_final_PGV=i_fir_list->last->utc_nanosecond; //utc时间纳秒
					//	}
					if(i_fir_list->band_pass_dataD[4]>band_pass_data->final_PGD)
						{
						band_pass_data->final_PGD=i_fir_list->band_pass_dataD[4];
						band_pass_data->utc_second_final_PGD=i_fir_list->utc_second; //utc时间秒
						band_pass_data->utc_nanosecond_final_PGD=i_fir_list->utc_nanosecond; //utc时间纳秒
						}
					//else
					//	{
					//	band_pass_data->final_PGD=i_fir_list->last->final_PGD;
					//	band_pass_data->utc_second_final_PGD=i_fir_list->last->utc_second; //utc时间秒
					//	band_pass_data->utc_nanosecond_final_PGD=i_fir_list->last->utc_nanosecond; //utc时间纳秒
					//	}
					if(*count==timespan*50)

						{
						*count=1;
						return 2;
						}
					else
						{


						*count+=1;
						}



					}
				}
			return 1;
			}
	else if(i_fir_list->sample_rata==100&&band_pass_flag==0)
		{
		firLength=191;
		double Fir[191]={-1.0096108503799409e-03,
			-1.3950482601184145e-03,
			-2.2527488512072823e-03,
			-2.9478325266950626e-03,
			-2.9241282941009309e-03,
			-2.1964877583498382e-03,
			-1.3932186866449717e-03,
			-1.2683702309322061e-03,
			-2.0587294816955936e-03,
			-3.2307108495224480e-03,
			-3.8728518839418980e-03,
			-3.4512139742405856e-03,
			-2.3045215990983815e-03,
			-1.4270997345429326e-03,
			-1.6661388536950924e-03,
			-2.9780040916341042e-03,
			-4.3915096269727697e-03,
			-4.7685469089617048e-03,
			-3.7708238360917514e-03,
			-2.2154696854772564e-03,
			-1.4639880273141859e-03,
			-2.2943598211664952e-03,
			-4.1868456476722750e-03,
			-5.6701595913976281e-03,
			-5.5118721440431517e-03,
			-3.7942060764075075e-03,
			-1.9423711063670419e-03,
			-1.6026343679009464e-03,
			-3.2475373165740244e-03,
			-5.6846723133800129e-03,
			-6.9564131921419870e-03,
			-5.9605784961465312e-03,
			-3.4555395583318526e-03,
			-1.5446848893690436e-03,
			-1.9729728155626920e-03,
			-4.6090756549750133e-03,
			-7.4249335517157806e-03,
			-8.0957173947760943e-03,
			-5.9658402480322953e-03,
			-2.7259882160986819e-03,
			-1.1326443571281007e-03,
			-2.7279163951726233e-03,
			-6.4379538633729206e-03,
			-9.3078652451388747e-03,
			-8.8937043531173623e-03,
			-5.3843686320969916e-03,
			-1.6236386602042371e-03,
			-8.6795329695463013e-04,
			-4.0358532878403434e-03,
			-8.7602540790057502e-03,
			-1.1177665546498936e-02,
			-9.1197841612625230e-03,
			-4.0841129832286773e-03,
			-2.1710181588843413e-04,
			-9.6506787169073674e-04,
			-6.0820299780298100e-03,
			-1.1571060989487639e-02,
			-1.2821283663388564e-02,
			-8.4992305065876590e-03,
			-1.9327687604891049e-03,
			1.3784923032710335e-03,
			-1.7050246429269466e-03,
			-9.0969663617697753e-03,
			-1.4856515017832844e-02,
			-1.3958829431456619e-02,
			-6.6666698386317613e-03,
			1.2562792472859616e-03,
			3.0131140370235859e-03,
			-3.4963797956000272e-03,
			-1.3462418900154581e-02,
			-1.8662168940531118e-02,
			-1.4193048970612182e-02,
			-2.9949266075571710e-03,
			5.9029683387124953e-03,
			4.5197236235567867e-03,
			-7.1079148261703534e-03,
			-2.0084767678293092e-02,
			-2.3311188040322776e-02,
			-1.2784038623879721e-02,
			4.0762178712041601e-03,
			1.3346915816899603e-02,
			5.7359043739684543e-03,
			-1.4720837908786619e-02,
			-3.2099574332974203e-02,
			-3.0404413640455791e-02,
			-7.3824560015768315e-03,
			2.0903522723558492e-02,
			3.0096061155356627e-02,
			6.5259347250955285e-03,
			-3.8462292200619481e-02,
			-7.0869487224785352e-02,
			-5.4107874682352952e-02,
			2.5259979404526650e-02,
			1.4370865641013861e-01,
			2.4982563870147181e-01,
			2.9230819959096299e-01,
			2.4982563870147181e-01,
			1.4370865641013864e-01,
			2.5259979404526654e-02,
			-5.4107874682352966e-02,
			-7.0869487224785366e-02,
			-3.8462292200619495e-02,
			6.5259347250955302e-03,
			3.0096061155356641e-02,
			2.0903522723558506e-02,
			-7.3824560015768349e-03,
			-3.0404413640455815e-02,
			-3.2099574332974223e-02,
			-1.4720837908786630e-02,
			5.7359043739684604e-03,
			1.3346915816899613e-02,
			4.0762178712041627e-03,
			-1.2784038623879739e-02,
			-2.3311188040322804e-02,
			-2.0084767678293116e-02,
			-7.1079148261703612e-03,
			4.5197236235567911e-03,
			5.9029683387125048e-03,
			-2.9949266075571736e-03,
			-1.4193048970612207e-02,
			-1.8662168940531153e-02,
			-1.3462418900154604e-02,
			-3.4963797956000320e-03,
			3.0131140370235911e-03,
			1.2562792472859642e-03,
			-6.6666698386317743e-03,
			-1.3958829431456649e-02,
			-1.4856515017832875e-02,
			-9.0969663617697927e-03,
			-1.7050246429269492e-03,
			1.3784923032710370e-03,
			-1.9327687604891093e-03,
			-8.4992305065876780e-03,
			-1.2821283663388595e-02,
			-1.1571060989487670e-02,
			-6.0820299780298256e-03,
			-9.6506787169073935e-04,
			-2.1710181588843587e-04,
			-4.0841129832286877e-03,
			-9.1197841612625508e-03,
			-1.1177665546498967e-02,
			-8.7602540790057779e-03,
			-4.0358532878403556e-03,
			-8.6795329695463187e-04,
			-1.6236386602042424e-03,
			-5.3843686320970089e-03,
			-8.8937043531173883e-03,
			-9.3078652451389042e-03,
			-6.4379538633729406e-03,
			-2.7279163951726333e-03,
			-1.1326443571281050e-03,
			-2.7259882160986914e-03,
			-5.9658402480323187e-03,
			-8.0957173947761238e-03,
			-7.4249335517158083e-03,
			-4.6090756549750297e-03,
			-1.9729728155626998e-03,
			-1.5446848893690497e-03,
			-3.4555395583318661e-03,
			-5.9605784961465555e-03,
			-6.9564131921420139e-03,
			-5.6846723133800337e-03,
			-3.2475373165740378e-03,
			-1.6026343679009538e-03,
			-1.9423711063670500e-03,
			-3.7942060764075232e-03,
			-5.5118721440431751e-03,
			-5.6701595913976532e-03,
			-4.1868456476722923e-03,
			-2.2943598211665065e-03,
			-1.4639880273141920e-03,
			-2.2154696854772668e-03,
			-3.7708238360917696e-03,
			-4.7685469089617273e-03,
			-4.3915096269727888e-03,
			-2.9780040916341185e-03,
			-1.6661388536951005e-03,
			-1.4270997345429397e-03,
			-2.3045215990983915e-03,
			-3.4512139742406017e-03,
			-3.8728518839419141e-03,
			-3.2307108495224631e-03,
			-2.0587294816956044e-03,
			-1.2683702309322122e-03,
			-1.3932186866449782e-03,
			-2.1964877583498486e-03,
			-2.9241282941009409e-03,
			-2.9478325266950032e-03,
			-2.2527488512072988e-03,
			-1.3950482601184288e-03,
			-1.0096108503799466e-03,



			};
		if(*trig!=0)
			{


			if(PGV_type==1)
				{
				datamid=0.0;
				now_fir=i_fir_list;
				for(int i=0;i<firLength;i++)
					{
					datamid=datamid+now_fir->dataV[4]*Fir[i];
					now_fir=now_fir->last;
					}
				i_fir_list->band_pass_dataV[4]=datamid;



				datamid=0.0;
				now_fir=i_fir_list;
				for(int i=0;i<firLength;i++)
					{
					datamid=datamid+now_fir->dataD[4]*Fir[i];
					now_fir=now_fir->last;
					}
				i_fir_list->band_pass_dataD[4]=datamid;

				}
			else if(PGV_type==2)
				{
				for(int j=0;j<3;j++)
					{


					datamid=0.0;
					now_fir=i_fir_list;
					for(int i=0;i<firLength;i++)
						{
						datamid=datamid+now_fir->dataV[j]*Fir[i];
						now_fir=now_fir->last;
						}
					i_fir_list->band_pass_dataV[j]=datamid;

					}
				i_fir_list->band_pass_dataV[4]=pow((double)(i_fir_list->band_pass_dataV[0]*i_fir_list->band_pass_dataV[0]+i_fir_list->band_pass_dataV[1]*i_fir_list->band_pass_dataV[1]),0.5);
				for(int j=0;j<3;j++)
					{


					datamid=0.0;
					now_fir=i_fir_list;
					for(int i=0;i<firLength;i++)
						{
						datamid=datamid+now_fir->dataD[j]*Fir[i];
						now_fir=now_fir->last;
						}
					i_fir_list->band_pass_dataD[j]=datamid;
					}
				i_fir_list->band_pass_dataD[4]=pow((double)(i_fir_list->band_pass_dataD[0]*i_fir_list->band_pass_dataD[0]+i_fir_list->band_pass_dataD[1]*i_fir_list->band_pass_dataD[1]),0.5);
				}

			if(*trig==1)
				{
				for(jjj=0;jjj<3;jjj++)
					{
					band_pass_data->final_PGA_component[jjj]=abs(i_fir_list->band_pass_data[jjj]);
					band_pass_data->final_PGV_component[jjj]=abs(i_fir_list->band_pass_dataV[jjj]);
					band_pass_data->final_PGD_component[jjj]=abs(i_fir_list->band_pass_dataD[jjj]);
					}
				band_pass_data->final_PGA=i_fir_list->band_pass_data[j];
				band_pass_data->final_PGV=i_fir_list->band_pass_dataV[j];
				band_pass_data->final_PGD=i_fir_list->band_pass_dataD[j];
				*trig=2;
				*count+=1;
				}
			else
				{
				for(jjj=0;jjj<3;jjj++)
					{
					if(abs(i_fir_list->band_pass_data[jjj])>band_pass_data->final_PGA_component[jjj])
						band_pass_data->final_PGA_component[jjj]=abs(i_fir_list->band_pass_data[jjj]);
					if(abs(i_fir_list->band_pass_dataV[jjj])>band_pass_data->final_PGV_component[jjj])
						band_pass_data->final_PGV_component[jjj]=abs(i_fir_list->band_pass_dataV[jjj]);
					if(abs(i_fir_list->band_pass_dataD[jjj])>band_pass_data->final_PGD_component[jjj])
						band_pass_data->final_PGD_component[jjj]=abs(i_fir_list->band_pass_dataD[jjj]);
					}

				if(i_fir_list->band_pass_data[4]>band_pass_data->final_PGA)
					{
					band_pass_data->final_PGA=i_fir_list->band_pass_data[4];
					band_pass_data->utc_second_final_PGA=i_fir_list->utc_second; //utc时间秒
					band_pass_data->utc_nanosecond_final_PGA=i_fir_list->utc_nanosecond; //utc时间纳秒

					}
				if(i_fir_list->band_pass_dataV[4]>band_pass_data->final_PGV)
					{
					band_pass_data->final_PGV=i_fir_list->band_pass_dataV[4];
					band_pass_data->utc_second_final_PGV=i_fir_list->utc_second; //utc时间秒
					band_pass_data->utc_nanosecond_final_PGV=i_fir_list->utc_nanosecond; //utc时间纳秒
					}
				if(i_fir_list->band_pass_dataD[4]>band_pass_data->final_PGD)
					{
					band_pass_data->final_PGD=i_fir_list->band_pass_dataD[4];
					band_pass_data->utc_second_final_PGD=i_fir_list->utc_second; //utc时间秒
					band_pass_data->utc_nanosecond_final_PGD=i_fir_list->utc_nanosecond; //utc时间纳秒
					}
				if(*count==timespan*100)

					{
					*count=1;
					return 2;
					}
				else
					{


					*count+=1;
					}



				}
			}
		return 1;
		}
	else if(i_fir_list->sample_rata==100&&band_pass_flag==1)
				{
				firLength=301;
				double Fir[301]={-1.3934901977829615e-03,
					-1.8159417519245753e-03,
					-2.1047061992379365e-03,
					-2.1499567281675455e-03,
					-1.9319560065597574e-03,
					-1.5336077824486048e-03,
					-1.1131764610959451e-03,
					-8.4586773282764453e-04,
					-8.5571492559512663e-04,
					-1.1641429939370940e-03,
					-1.6763388826177095e-03,
					-2.2127045829863049e-03,
					-2.5752520825855834e-03,
					-2.6245209995463341e-03,
					-2.3372518475216625e-03,
					-1.8211766253946077e-03,
					-1.2790312324263129e-03,
					-9.3350462293830287e-04,
					-9.4077162689895221e-04,
					-1.3260577655623985e-03,
					-1.9675936202030414e-03,
					-2.6375233932370771e-03,
					-3.0863445849885914e-03,
					-3.1397319857095246e-03,
					-2.7703067122125825e-03,
					-2.1150445844176149e-03,
					-1.4290329171544275e-03,
					-9.9084931593346887e-04,
					-9.9450274977262157e-04,
					-1.4697037836401406e-03,
					-2.2629691037629874e-03,
					-3.0896434805398041e-03,
					-3.6395636772003424e-03,
					-3.6973125284921167e-03,
					-3.2306007769072587e-03,
					-2.4107945111442089e-03,
					-1.5546360967761310e-03,
					-1.0065972753527060e-03,
					-1.0053738031440373e-03,
					-1.5860839569270741e-03,
					-2.5579388666770995e-03,
					-3.5692958764463449e-03,
					-4.2383209384534298e-03,
					-4.3009144849360951e-03,
					-3.7187912246025431e-03,
					-2.7038140209237104e-03,
					-1.6456047543273620e-03,
					-9.6672554881740698e-04,
					-9.5902862506833195e-04,
					-1.6643298430027724e-03,
					-2.8478287426786748e-03,
					-4.0784458396371038e-03,
					-4.8890672246237271e-03,
					-4.9573843372280107e-03,
					-4.2375689803696064e-03,
					-2.9893918921230977e-03,
					-1.6892661355065719e-03,
					-8.5315338472186943e-04,
					-8.3687322528905046e-04,
					-1.6908134983387932e-03,
					-3.1279156197627396e-03,
					-4.6219899287065240e-03,
					-5.6032676672302818e-03,
					-5.6788440317752488e-03,
					-4.7930527289633748e-03,
					-3.2628185712932083e-03,
					-1.6691643874906282e-03,
					-6.4136490201577951e-04,
					-6.1355241309235965e-04,
					-1.6475301337705620e-03,
					-3.3935290783997270e-03,
					-5.2097909555231794e-03,
					-6.4008237800856037e-03,
					-6.4863194218901269e-03,
					-5.3972072803381739e-03,
					-3.5194883154958849e-03,
					-1.5625414496258179e-03,
					-2.9599484306628491e-04,
					-2.5223929370514470e-04,
					-1.5090285192966766e-03,
					-3.6401534025372944e-03,
					-5.8603941401391838e-03,
					-7.3164248762943827e-03,
					-7.4165259229421188e-03,
					-6.0723665880914419e-03,
					-3.7550004343177984e-03,
					-1.3353600892311249e-03,
					2.3788693116028032e-04,
					3.0473249946160190e-04,
					-1.2362210827844447e-03,
					-3.8635274170506517e-03,
					-6.6084146410208293e-03,
					-8.4123588516069620e-03,
					-8.5356836076430822e-03,
					-6.8604886253879381e-03,
					-3.9652570935789742e-03,
					-9.3168387924516256e-04,
					1.0534705820681239e-03,
					1.1561602642038765e-03,
					-7.6279308836732713e-04,
					-4.0597396392069423e-03,
					-7.5208401297042239e-03,
					-9.8072001726968727e-03,
					-9.9708294562862142e-03,
					-7.8443429100596568e-03,
					-4.1465552109397862e-03,
					-2.4844224798785294e-04,
					2.3271192130121512e-03,
					2.4913851040193188e-03,
					3.8417223566559994e-05,
					-4.2253163338121661e-03,
					-8.7382536865734783e-03,
					-1.1749625514224836e-02,
					-1.1991733098263043e-02,
					-9.2038390171244738e-03,
					-4.2956701090024274e-03,
					9.3449579474537436e-04,
					4.4450971219227456e-03,
					4.7325123287797061e-03,
					1.4542781444556027e-03,
					-4.3573002295476306e-03,
					-1.0601141815511635e-02,
					-1.4850187602870650e-02,
					-1.5274622891964655e-02,
					-1.1402122422123587e-02,
					-4.4099287906940446e-03,
					3.1992627279992338e-03,
					8.4610861449535505e-03,
					9.0647035001510286e-03,
					4.3128967563563032e-03,
					-4.4533178832161045e-03,
					-1.4174787943096560e-02,
					-2.1085357894361143e-02,
					-2.2096384652325083e-02,
					-1.6069207429197555e-02,
					-4.4872709594348387e-03,
					8.7849658638558172e-03,
					1.8665591162767350e-02,
					2.0618219193568481e-02,
					1.2446574643172092e-02,
					-4.5116339631119861e-03,
					-2.5272940970094884e-02,
					-4.2331277335992394e-02,
					-4.7764061273754821e-02,
					-3.5721208137345535e-02,
					-4.5262962159261927e-03,
					4.2308025951629580e-02,
					9.6544322689801162e-02,
					1.4711299332650787e-01,
					1.8293486636295642e-01,
					1.9587085363289147e-01,
					1.8293486636295639e-01,
					1.4711299332650785e-01,
					9.6544322689801135e-02,
					4.2308025951629566e-02,
					-4.5262962159261918e-03,
					-3.5721208137345535e-02,
					-4.7764061273754814e-02,
					-4.2331277335992387e-02,
					-2.5272940970094880e-02,
					-4.5116339631119853e-03,
					1.2446574643172089e-02,
					2.0618219193568474e-02,
					1.8665591162767347e-02,
					8.7849658638558138e-03,
					-4.4872709594348361e-03,
					-1.6069207429197544e-02,
					-2.2096384652325073e-02,
					-2.1085357894361126e-02,
					-1.4174787943096552e-02,
					-4.4533178832161010e-03,
					4.3128967563563006e-03,
					9.0647035001510234e-03,
					8.4610861449535436e-03,
					3.1992627279992312e-03,
					-4.4099287906940411e-03,
					-1.1402122422123580e-02,
					-1.5274622891964643e-02,
					-1.4850187602870640e-02,
					-1.0601141815511625e-02,
					-4.3573002295476280e-03,
					1.4542781444556018e-03,
					4.7325123287797017e-03,
					4.4450971219227413e-03,
					9.3449579474537349e-04,
					-4.2956701090024231e-03,
					-9.2038390171244669e-03,
					-1.1991733098263025e-02,
					-1.1749625514224822e-02,
					-8.7382536865734661e-03,
					-4.2253163338121600e-03,
					3.8417223566559994e-05,
					2.4913851040193153e-03,
					2.3271192130121469e-03,
					-2.4844224798785381e-04,
					-4.1465552109397810e-03,
					-7.8443429100596446e-03,
					-9.9708294562862020e-03,
					-9.8072001726968588e-03,
					-7.5208401297042135e-03,
					-4.0597396392069380e-03,
					-7.6279308836732583e-04,
					1.1561602642038756e-03,
					1.0534705820681221e-03,
					-9.3168387924516213e-04,
					-3.9652570935789682e-03,
					-6.8604886253879251e-03,
					-8.5356836076430683e-03,
					-8.4123588516069481e-03,
					-6.6084146410208154e-03,
					-3.8635274170506460e-03,
					-1.2362210827844425e-03,
					3.0473249946160017e-04,
					2.3788693116027988e-04,
					-1.3353600892311228e-03,
					-3.7550004343177915e-03,
					-6.0723665880914298e-03,
					-7.4165259229421032e-03,
					-7.3164248762943671e-03,
					-5.8603941401391716e-03,
					-3.6401534025372871e-03,
					-1.5090285192966731e-03,
					-2.5223929370514383e-04,
					-2.9599484306628491e-04,
					-1.5625414496258144e-03,
					-3.5194883154958771e-03,
					-5.3972072803381609e-03,
					-6.4863194218901122e-03,
					-6.4008237800855889e-03,
					-5.2097909555231664e-03,
					-3.3935290783997183e-03,
					-1.6475301337705575e-03,
					-6.1355241309235748e-04,
					-6.4136490201577821e-04,
					-1.6691643874906245e-03,
					-3.2628185712932005e-03,
					-4.7930527289633627e-03,
					-5.6788440317752332e-03,
					-5.6032676672302679e-03,
					-4.6219899287065127e-03,
					-3.1279156197627305e-03,
					-1.6908134983387882e-03,
					-8.3687322528904872e-04,
					-8.5315338472186726e-04,
					-1.6892661355065672e-03,
					-2.9893918921230908e-03,
					-4.2375689803695943e-03,
					-4.9573843372279977e-03,
					-4.8890672246237124e-03,
					-4.0784458396370933e-03,
					-2.8478287426786674e-03,
					-1.6643298430027674e-03,
					-9.5902862506832913e-04,
					-9.6672554881740416e-04,
					-1.6456047543273575e-03,
					-2.7038140209237008e-03,
					-3.7187912246025327e-03,
					-4.3009144849360829e-03,
					-4.2383209384534160e-03,
					-3.5692958764463336e-03,
					-2.5579388666770917e-03,
					-1.5860839569270702e-03,
					-1.0053738031440341e-03,
					-1.0065972753527028e-03,
					-1.5546360967761263e-03,
					-2.4107945111442020e-03,
					-3.2306007769072496e-03,
					-3.6973125284921046e-03,
					-3.6395636772003298e-03,
					-3.0896434805397937e-03,
					-2.2629691037629792e-03,
					-1.4697037836401362e-03,
					-9.9450274977261875e-04,
					-9.9084931593346518e-04,
					-1.4290329171544223e-03,
					-2.1150445844176089e-03,
					-2.7703067122125742e-03,
					-3.1397319857095142e-03,
					-3.0863445849885810e-03,
					-2.6375233932370672e-03,
					-1.9675936202030344e-03,
					-1.3260577655623941e-03,
					-9.4077162689894884e-04,
					-9.3350462293829940e-04,
					-1.2790312324263090e-03,
					-1.8211766253946012e-03,
					-2.3372518475216551e-03,
					-2.6245209995463237e-03,
					-2.5752520825855730e-03,
					-2.2127045829862962e-03,
					-1.6763388826177036e-03,
					-1.1641429939370888e-03,
					-8.5571492559512359e-04,
					-8.4586773282764138e-04,
					-1.1131764610959410e-03,
					-1.5336077824486000e-03,
					-1.9319560065597509e-03,
					-2.1499567281675781e-03,
					-2.1047061992379612e-03,
					-1.8159417519245733e-03,
					-1.3934901977829676e-03,



					};
				if(*trig!=0)
					{


					if(PGV_type==1)
						{
						datamid=0.0;
						now_fir=i_fir_list;
						for(int i=0;i<firLength;i++)
							{
							datamid=datamid+now_fir->dataV[4]*Fir[i];
							now_fir=now_fir->last;
							}
						i_fir_list->band_pass_dataV[4]=datamid;



						datamid=0.0;
						now_fir=i_fir_list;
						for(int i=0;i<firLength;i++)
							{
							datamid=datamid+now_fir->dataD[4]*Fir[i];
							now_fir=now_fir->last;
							}
						i_fir_list->band_pass_dataD[4]=datamid;

						}
					else if(PGV_type==2)
						{
						for(int j=0;j<3;j++)
							{


							datamid=0.0;
							now_fir=i_fir_list;
							for(int i=0;i<firLength;i++)
								{
								datamid=datamid+now_fir->dataV[j]*Fir[i];
								now_fir=now_fir->last;
								}
							i_fir_list->band_pass_dataV[j]=datamid;

							}
						i_fir_list->band_pass_dataV[4]=pow((double)(i_fir_list->band_pass_dataV[0]*i_fir_list->band_pass_dataV[0]+i_fir_list->band_pass_dataV[1]*i_fir_list->band_pass_dataV[1]),0.5);
						for(int j=0;j<3;j++)
							{


							datamid=0.0;
							now_fir=i_fir_list;
							for(int i=0;i<firLength;i++)
								{
								datamid=datamid+now_fir->dataD[j]*Fir[i];
								now_fir=now_fir->last;
								}
							i_fir_list->band_pass_dataD[j]=datamid;
							}
						i_fir_list->band_pass_dataD[4]=pow((double)(i_fir_list->band_pass_dataD[0]*i_fir_list->band_pass_dataD[0]+i_fir_list->band_pass_dataD[1]*i_fir_list->band_pass_dataD[1]),0.5);
						}

					if(*trig==1)
						{
						for(jjj=0;jjj<3;jjj++)
							{
							band_pass_data->final_PGA_component[jjj]=abs(i_fir_list->band_pass_data[jjj]);
							band_pass_data->final_PGV_component[jjj]=abs(i_fir_list->band_pass_dataV[jjj]);
							band_pass_data->final_PGD_component[jjj]=abs(i_fir_list->band_pass_dataD[jjj]);
							}
						band_pass_data->final_PGA=i_fir_list->band_pass_data[j];
						band_pass_data->final_PGV=i_fir_list->band_pass_dataV[j];
						band_pass_data->final_PGD=i_fir_list->band_pass_dataD[j];
						*trig=2;
						*count+=1;
						}
					else
						{
						for(jjj=0;jjj<3;jjj++)
							{
							if(abs(i_fir_list->band_pass_data[jjj])>band_pass_data->final_PGA_component[jjj])
								band_pass_data->final_PGA_component[jjj]=abs(i_fir_list->band_pass_data[jjj]);
							if(abs(i_fir_list->band_pass_dataV[jjj])>band_pass_data->final_PGV_component[jjj])
								band_pass_data->final_PGV_component[jjj]=abs(i_fir_list->band_pass_dataV[jjj]);
							if(abs(i_fir_list->band_pass_dataD[jjj])>band_pass_data->final_PGD_component[jjj])
								band_pass_data->final_PGD_component[jjj]=abs(i_fir_list->band_pass_dataD[jjj]);
							}

						if(i_fir_list->band_pass_data[4]>band_pass_data->final_PGA)
							{
							band_pass_data->final_PGA=i_fir_list->band_pass_data[4];
							band_pass_data->utc_second_final_PGA=i_fir_list->utc_second; //utc时间秒
							band_pass_data->utc_nanosecond_final_PGA=i_fir_list->utc_nanosecond; //utc时间纳秒

							}
						if(i_fir_list->band_pass_dataV[4]>band_pass_data->final_PGV)
							{
							band_pass_data->final_PGV=i_fir_list->band_pass_dataV[4];
							band_pass_data->utc_second_final_PGV=i_fir_list->utc_second; //utc时间秒
							band_pass_data->utc_nanosecond_final_PGV=i_fir_list->utc_nanosecond; //utc时间纳秒
							}
						if(i_fir_list->band_pass_dataD[4]>band_pass_data->final_PGD)
							{
							band_pass_data->final_PGD=i_fir_list->band_pass_dataD[4];
							band_pass_data->utc_second_final_PGD=i_fir_list->utc_second; //utc时间秒
							band_pass_data->utc_nanosecond_final_PGD=i_fir_list->utc_nanosecond; //utc时间纳秒
							}
						if(*count==timespan*100)

							{
							*count=1;
							return 2;
							}
						else
							{


							*count+=1;
							}



						}
					}
				return 1;
		}
	else if(i_fir_list->sample_rata==200&&band_pass_flag==0)
		{
			
			alg_locat();

		firLength=301;
		//if(dataNum<301)
		//	{
		//	return -1;
		//	}
		double Fir[301]={-1.5738968347689628e-04,
			-1.7735491639294824e-04,
			-2.1736124094126334e-04,
			-2.7483905489700012e-04,
			-3.4266813193163193e-04,
			-4.1030106015100642e-04,
			-4.6590026089908369e-04,
			-4.9908565647348161e-04,
			-5.0368485858866155e-04,
			-4.7980096272096865e-04,
			-4.3459912811287315e-04,
			-3.8145932023061683e-04,
			-3.3750551620498548e-04,
			-3.1992389949451273e-04,
			-3.4182841427864982e-04,
			-4.0862956128619334e-04,
			-5.1584657092479044e-04,
			-6.4905509995696496e-04,
			-7.8621869932361810e-04,
			-9.0210075583715586e-04,
			-9.7391805542624665e-04,
			-9.8700968544837217e-04,
			-9.3916478239759282e-04,
			-8.4243680789333406e-04,
			-7.2175694572392348e-04,
			-6.1035700575723644e-04,
			-5.4277523357811595e-04,
			-5.4686940852928067e-04,
			-6.3663386121590247e-04,
			-8.0759555645765300e-04,
			-1.0361182005924534e-03,
			-1.2831404037890676e-03,
			-1.5018735401092786e-03,
			-1.6480075080620943e-03,
			-1.6902522160705587e-03,
			-1.6187724217614032e-03,
			-1.4493561975616916e-03,
			-1.2219729859712122e-03,
			-9.9357826868147307e-04,
			-8.2635727901226479e-04,
			-7.7376662014393995e-04,
			-8.6744203333462462e-04,
			-1.1080893063261379e-03,
			-1.4627990304460733e-03,
			-1.8699254737555102e-03,
			-2.2510004567963386e-03,
			-2.5274762640143944e-03,
			-2.6387982693635644e-03,
			-2.5577291433000224e-03,
			-2.2991713422791313e-03,
			-1.9199554375127431e-03,
			-1.5089616520395268e-03,
			-1.1691295962667500e-03,
			-9.9489997697284663e-04,
			-1.0499463509793807e-03,
			-1.3503416137936456e-03,
			-1.8574229089935940e-03,
			-2.4826907958067261e-03,
			-3.1044754803961948e-03,
			-3.5933784465069858e-03,
			-3.8412720032523487e-03,
			-3.7874595744862814e-03,
			-3.4358093523187173e-03,
			-2.8583194491591827e-03,
			-2.1833724736893233e-03,
			-1.5703255973366405e-03,
			-1.1753198885291142e-03,
			-1.1155270970319925e-03,
			-1.4398884580665875e-03,
			-2.1134488140001156e-03,
			-3.0197519628140561e-03,
			-3.9819340454248483e-03,
			-4.7989298359828224e-03,
			-5.2895315090100694e-03,
			-5.3347847421739408e-03,
			-4.9089772067057768e-03,
			-4.0914494807903403e-03,
			-3.0553263232042977e-03,
			-2.0342643861179957e-03,
			-1.2733736807093051e-03,
			-9.7444743993873348e-04,
			-1.2475579059199422e-03,
			-2.0803813561700931e-03,
			-3.3333020239832397e-03,
			-4.7630034661209093e-03,
			-6.0709747972200970e-03,
			-6.9675036714509026e-03,
			-7.2376487999995407e-03,
			-6.7944115044867989e-03,
			-5.7063144258142880e-03,
			-4.1915922126739454e-03,
			-2.5782559165073099e-03,
			-1.2369414329412996e-03,
			-5.0000439657026010e-04,
			-5.8423897384719532e-04,
			-1.5348385257546453e-03,
			-3.2045099423030948e-03,
			-5.2745989273480509e-03,
			-7.3160653117819293e-03,
			-8.8790713655859960e-03,
			-9.5928420397288355e-03,
			-9.2540312860429144e-03,
			-7.8830675166212654e-03,
			-5.7338544840285889e-03,
			-3.2517317719462481e-03,
			-9.8584385474959567e-04,
			5.2737308823975636e-04,
			8.8539160937573045e-04,
			-8.1278022656460546e-05,
			-2.2555652814956074e-03,
			-5.2408116952406677e-03,
			-8.4306412604161132e-03,
			-1.1130333116173553e-02,
			-1.2706151017152620e-02,
			-1.2731360484584070e-02,
			-1.1096337143008480e-02,
			-8.0559256539294525e-03,
			-4.1992280006084327e-03,
			-3.4304817834827792e-04,
			2.6329471431176831e-03,
			3.9772854451277720e-03,
			3.2439302636812226e-03,
			4.1585700052789186e-04,
			-4.0560792879092526e-03,
			-9.3139281317865166e-03,
			-1.4247613714347402e-02,
			-1.7717974336771903e-02,
			-1.8805230945223246e-02,
			-1.7031601742540699e-02,
			-1.2508339273051561e-02,
			-5.9695509104043653e-03,
			1.3240748146035209e-03,
			7.8028250387371500e-03,
			1.1892350786033203e-02,
			1.2350080720461974e-02,
			8.5682310423650879e-03,
			7.7979549076405419e-04,
			-9.8815104670206631e-03,
			-2.1491211233783846e-02,
			-3.1606466363625955e-02,
			-3.7661758554831441e-02,
			-3.7422874756706775e-02,
			-2.9419394015175061e-02,
			-1.3275490993334308e-02,
			1.0126511572124210e-02,
			3.8687978116648487e-02,
			6.9343825778920370e-02,
			9.8485912360757433e-02,
			1.2249350941875659e-01,
			1.3828375204993210e-01,
			1.4378957268385817e-01,
			1.3828375204993198e-01,
			1.2249350941875640e-01,
			9.8485912360757155e-02,
			6.9343825778920107e-02,
			3.8687978116648321e-02,
			1.0126511572124158e-02,
			-1.3275490993334225e-02,
			-2.9419394015174839e-02,
			-3.7422874756706463e-02,
			-3.7661758554831101e-02,
			-3.1606466363625629e-02,
			-2.1491211233783607e-02,
			-9.8815104670205468e-03,
			7.7979549076404465e-04,
			8.5682310423649734e-03,
			1.2350080720461792e-02,
			1.1892350786033016e-02,
			7.8028250387370190e-03,
			1.3240748146034983e-03,
			-5.9695509104042534e-03,
			-1.2508339273051316e-02,
			-1.7031601742540346e-02,
			-1.8805230945222844e-02,
			-1.7717974336771518e-02,
			-1.4247613714347076e-02,
			-9.3139281317862910e-03,
			-4.0560792879091503e-03,
			4.1585700052788145e-04,
			3.2439302636811385e-03,
			3.9772854451276627e-03,
			2.6329471431176059e-03,
			-3.4304817834826751e-04,
			-4.1992280006083061e-03,
			-8.0559256539291975e-03,
			-1.1096337143008119e-02,
			-1.2731360484583639e-02,
			-1.2706151017152188e-02,
			-1.1130333116173159e-02,
			-8.4306412604158061e-03,
			-5.2408116952404717e-03,
			-2.2555652814955185e-03,
			-8.1278022656457077e-05,
			8.8539160937569489e-04,
			5.2737308823973467e-04,
			-9.8584385474955491e-04,
			-3.2517317719461085e-03,
			-5.7338544840283339e-03,
			-7.8830675166209115e-03,
			-9.2540312860424894e-03,
			-9.5928420397283845e-03,
			-8.8790713655855676e-03,
			-7.3160653117815720e-03,
			-5.2745989273477829e-03,
			-3.2045099423029331e-03,
			-1.5348385257545655e-03,
			-5.8423897384716496e-04,
			-5.0000439657023278e-04,
			-1.2369414329412311e-03,
			-2.5782559165071638e-03,
			-4.1915922126737068e-03,
			-5.7063144258139575e-03,
			-6.7944115044863947e-03,
			-7.2376487999991001e-03,
			-6.9675036714504724e-03,
			-6.0709747972197223e-03,
			-4.7630034661206057e-03,
			-3.3333020239830237e-03,
			-2.0803813561699586e-03,
			-1.2475579059198585e-03,
			-9.7444743993866712e-04,
			-1.2733736807092175e-03,
			-2.0342643861178543e-03,
			-3.0553263232040791e-03,
			-4.0914494807900480e-03,
			-4.9089772067054177e-03,
			-5.3347847421735478e-03,
			-5.2895315090096687e-03,
			-4.7989298359824538e-03,
			-3.9819340454245395e-03,
			-3.0197519628138176e-03,
			-2.1134488139999473e-03,
			-1.4398884580664711e-03,
			-1.1155270970319006e-03,
			-1.1753198885290160e-03,
			-1.5703255973365086e-03,
			-2.1833724736891359e-03,
			-2.8583194491589325e-03,
			-3.4358093523184151e-03,
			-3.7874595744859440e-03,
			-3.8412720032520008e-03,
			-3.5933784465066554e-03,
			-3.1044754803959073e-03,
			-2.4826907958064920e-03,
			-1.8574229089934173e-03,
			-1.3503416137935148e-03,
			-1.0499463509792775e-03,
			-9.9489997697274797e-04,
			-1.1691295962666325e-03,
			-1.5089616520393724e-03,
			-1.9199554375125441e-03,
			-2.2991713422788910e-03,
			-2.5577291432997522e-03,
			-2.6387982693632807e-03,
			-2.5274762640141194e-03,
			-2.2510004567960896e-03,
			-1.8699254737553005e-03,
			-1.4627990304459074e-03,
			-1.1080893063260097e-03,
			-8.6744203333452400e-04,
			-7.7376662014384866e-04,
			-8.2635727901216656e-04,
			-9.9357826868135316e-04,
			-1.2219729859710637e-03,
			-1.4493561975615118e-03,
			-1.6187724217612007e-03,
			-1.6902522160703449e-03,
			-1.6480075080618820e-03,
			-1.5018735401090826e-03,
			-1.2831404037888989e-03,
			-1.0361182005923146e-03,
			-8.0759555645754327e-04,
			-6.3663386121581508e-04,
			-5.4686940852920413e-04,
			-5.4277523357803897e-04,
			-6.1035700575714949e-04,
			-7.2175694572381886e-04,
			-8.4243680789320959e-04,
			-9.3916478239745296e-04,
			-9.8700968544822277e-04,
			-9.7391805542609714e-04,
			-9.0210075583701556e-04,
			-7.8621869932349417e-04,
			-6.4905509995686088e-04,
			-5.1584657092470653e-04,
			-4.0862956128612563e-04,
			-3.4182841427859273e-04,
			-3.1992389949445831e-04,
			-3.3750551620492721e-04,
			-3.8145932023054988e-04,
			-4.3459912811279558e-04,
			-4.7980096272088148e-04,
			-5.0368485858856907e-04,
			-4.9908565647338837e-04,
			-4.6590026089899489e-04,
			-4.1030106015092717e-04,
			-3.4266813193156422e-04,
			-2.7483905489694477e-04,
			-2.1736124094121881e-04,
			-1.7735491639291114e-04,
			-1.5738968347686275e-04,


			};
						alg_locat();

		if(*trig!=0)
			{

			if(PGV_type==1)
				{
					alg_locat();
				datamid=0.0;
				now_fir=i_fir_list;
				for(int i=0;i<firLength;i++)
					{
					datamid=datamid+now_fir->dataV[4]*Fir[i];
					now_fir=now_fir->last;
					}
					alg_locat();
				i_fir_list->band_pass_dataV[4]=datamid;



				datamid=0.0;
				now_fir=i_fir_list;
				for(int i=0;i<firLength;i++)
					{
					datamid=datamid+now_fir->dataD[4]*Fir[i];
					now_fir=now_fir->last;
					}
				i_fir_list->band_pass_dataD[4]=datamid;
alg_locat();
				}
			else if(PGV_type==2)
				{
					alg_locat();
				for(int j=0;j<3;j++)
					{
alg_locat();

					datamid=0.0;
					now_fir=i_fir_list;
					for(int i=0;i<firLength;i++)
						{
						datamid=datamid+now_fir->dataV[j]*Fir[i];
						now_fir=now_fir->last;
						}
					i_fir_list->band_pass_dataV[j]=datamid;
alg_locat();
					}
				i_fir_list->band_pass_dataV[4]=pow((double)(i_fir_list->band_pass_dataV[0]*i_fir_list->band_pass_dataV[0]+i_fir_list->band_pass_dataV[1]*i_fir_list->band_pass_dataV[1]),0.5);
				for(int j=0;j<3;j++)
					{

alg_locat();
					datamid=0.0;
					now_fir=i_fir_list;
					for(int i=0;i<firLength;i++)
						{
						datamid=datamid+now_fir->dataD[j]*Fir[i];
						now_fir=now_fir->last;
						}
					i_fir_list->band_pass_dataD[j]=datamid;
					}
				i_fir_list->band_pass_dataD[4]=pow((double)(i_fir_list->band_pass_dataD[0]*i_fir_list->band_pass_dataD[0]+i_fir_list->band_pass_dataD[1]*i_fir_list->band_pass_dataD[1]),0.5);
				}
alg_locat();
			if(*trig==1)
				{
					alg_locat();
				for(jjj=0;jjj<3;jjj++)
					{
					band_pass_data->final_PGA_component[jjj]=abs(i_fir_list->band_pass_data[jjj]);
					band_pass_data->final_PGV_component[jjj]=abs(i_fir_list->band_pass_dataV[jjj]);
					band_pass_data->final_PGD_component[jjj]=abs(i_fir_list->band_pass_dataD[jjj]);
					alg_locat();
					}
					alg_locat();
				printf("j=%d\n",j);
				band_pass_data->final_PGA=i_fir_list->band_pass_data[j];//TODO j err
				alg_locat();
				band_pass_data->final_PGV=i_fir_list->band_pass_dataV[j];
				alg_locat();
				band_pass_data->final_PGD=i_fir_list->band_pass_dataD[j];
				alg_locat();
				*trig=2;
				alg_locat();
				*count+=1;
				alg_locat();
				alg_locat();
				}
			else
				{

alg_locat();
				for(jjj=0;jjj<3;jjj++)
					{
					if(abs(i_fir_list->band_pass_data[jjj])>band_pass_data->final_PGA_component[jjj])
						band_pass_data->final_PGA_component[jjj]=abs(i_fir_list->band_pass_data[jjj]);
					if(abs(i_fir_list->band_pass_dataV[jjj])>band_pass_data->final_PGV_component[jjj])
						band_pass_data->final_PGV_component[jjj]=abs(i_fir_list->band_pass_dataV[jjj]);
					if(abs(i_fir_list->band_pass_dataD[jjj])>band_pass_data->final_PGD_component[jjj])
						band_pass_data->final_PGD_component[jjj]=abs(i_fir_list->band_pass_dataD[jjj]);
					}

				if(i_fir_list->band_pass_data[4]>band_pass_data->final_PGA)
					{
					band_pass_data->final_PGA=i_fir_list->band_pass_data[4];
					band_pass_data->utc_second_final_PGA=i_fir_list->utc_second; //utc时间秒
					band_pass_data->utc_nanosecond_final_PGA=i_fir_list->utc_nanosecond; //utc时间纳秒

					}
				if(i_fir_list->band_pass_dataV[4]>band_pass_data->final_PGV)
					{
					band_pass_data->final_PGV=i_fir_list->band_pass_dataV[4];
					band_pass_data->utc_second_final_PGV=i_fir_list->utc_second; //utc时间秒
					band_pass_data->utc_nanosecond_final_PGV=i_fir_list->utc_nanosecond; //utc时间纳秒
					}
				if(i_fir_list->band_pass_dataD[4]>band_pass_data->final_PGD)
					{
					band_pass_data->final_PGD=i_fir_list->band_pass_dataD[4];
					band_pass_data->utc_second_final_PGD=i_fir_list->utc_second; //utc时间秒
					band_pass_data->utc_nanosecond_final_PGD=i_fir_list->utc_nanosecond; //utc时间纳秒
					}
				if(*count==timespan*200)

					{
					*count=1;
					alg_locat();
					return 2;
					}
				else
					{
alg_locat();

					*count+=1;
					}



				}
			}
		return 1;
		}
	else if(i_fir_list->sample_rata==200&&band_pass_flag==1)
		{
			alg_locat();
		firLength=541;
		//if(dataNum<301)
		//	{
		//	return -1;
		//	}
		double Fir[541]={-9.2099036038378532e-04,
			-7.8804826816350514e-04,
			-6.6560344061362866e-04,
			-5.6687547227462695e-04,
			-5.0305195443362731e-04,
			-4.8212458142003447e-04,
			-5.0801522925230963e-04,
			-5.8008622249482611e-04,
			-6.9309294300030314e-04,
			-8.3759419607064940e-04,
			-1.0007904473973064e-03,
			-1.1677166438897948e-03,
			-1.3226792145237351e-03,
			-1.4507998700714149e-03,
			-1.5395149164156340e-03,
			-1.5798796606311268e-03,
			-1.5675433814101799e-03,
			-1.5032900196379525e-03,
			-1.3930805785312536e-03,
			-1.2475813887482972e-03,
			-1.0812132551662164e-03,
			-9.1080504603679199e-04,
			-7.5397661608138985e-04,
			-6.2740578524895022e-04,
			-5.4514918710074384e-04,
			-5.1718531515799690e-04,
			-5.4832980445290475e-04,
			-6.3763933271860978e-04,
			-7.7837451944441324e-04,
			-9.5853814908441635e-04,
			-1.1619481531160475e-03,
			-1.3697506595220668e-03,
			-1.5622325114940184e-03,
			-1.7207597251464868e-03,
			-1.8296519661545247e-03,
			-1.8778052751213447e-03,
			-1.8598961554604190e-03,
			-1.7770380880220437e-03,
			-1.6368131495740684e-03,
			-1.4526618487227868e-03,
			-1.2426777301980229e-03,
			-1.0279134888687795e-03,
			-8.3035621871027751e-04,
			-6.7076573516148576e-04,
			-5.6658772472861297e-04,
			-5.3015062268610609e-04,
			-5.6733143777716296e-04,
			-6.7683314040934404e-04,
			-8.5015853938458274e-04,
			-1.0722982056842946e-03,
			-1.3230794514709333e-03,
			-1.5790565837738907e-03,
			-1.8157663436507616e-03,
			-2.0101324311648091e-03,
			-2.1427836247891099e-03,
			-2.2000535925879644e-03,
			-2.1754571889266504e-03,
			-2.0704856679088202e-03,
			-1.8946275302998378e-03,
			-1.6645966177239893e-03,
			-1.4028273596775336e-03,
			-1.1353710613629898e-03,
			-8.8938935754289306e-04,
			-6.9048502597345474e-04,
			-5.6013150133180209e-04,
			-5.1345808254886992e-04,
			-5.5761788704660181e-04,
			-6.9091251719947467e-04,
			-9.0277595818363658e-04,
			-1.1746371336738743e-03,
			-1.4815938115070816e-03,
			-1.7947487057919354e-03,
			-2.0839898846883352e-03,
			-2.3209490512706072e-03,
			-2.4818481466164577e-03,
			-2.5499498422015465e-03,
			-2.5173609120326093e-03,
			-2.3859964670479806e-03,
			-2.1675922672027342e-03,
			-1.8827443572853935e-03,
			-1.5590512379570593e-03,
			-1.2285242513423947e-03,
			-9.2450775236682355e-04,
			-6.7840413588373869e-04,
			-5.1652413990349946e-04,
			-4.5737696479821453e-04,
			-5.0967758766085958e-04,
			-6.7128325883947910e-04,
			-9.2918345122575824e-04,
			-1.2605657298138107e-03,
			-1.6348739179959412e-03,
			-2.0166749784740114e-03,
			-2.3690672229614647e-03,
			-2.6573034400949446e-03,
			-2.8522746288034552e-03,
			-2.9335066344339071e-03,
			-2.8913631482099544e-03,
			-2.7282208685875907e-03,
			-2.4584796038279720e-03,
			-2.1073826169179673e-03,
			-1.7087397545895546e-03,
			-1.3017563556190544e-03,
			-9.2726353456713112e-04,
			-6.2371066482157266e-04,
			-4.2331173523314162e-04,
			-3.4872998370800008e-04,
			-4.1063976990715308e-04,
			-6.0642476251474388e-04,
			-9.2016438994335105e-04,
			-1.3239361803047587e-03,
			-1.7803320420881636e-03,
			-2.2459644154696938e-03,
			-2.6756357782218845e-03,
			-3.0267727437604114e-03,
			-3.2636916931282249e-03,
			-3.3612706949638629e-03,
			-3.3076524683700241e-03,
			-3.1056912475649718e-03,
			-2.7729746824372565e-03,
			-2.3403892837598060e-03,
			-1.8493412139778430e-03,
			-1.3478794014496636e-03,
			-8.8608150223198922e-04,
			-5.1114349515264787e-04,
			-2.6265206842258714e-04,
			-1.6851079481957512e-04,
			-2.4193625100973236e-04,
			-4.7984317191924781e-04,
			-8.6280715728713074e-04,
			-1.3566415625985743e-03,
			-1.9154665600813400e-03,
			-2.4859984617802782e-03,
			-3.0126612251738065e-03,
			-3.4430325498542480e-03,
			-3.7330937104403427e-03,
			-3.8517604832979464e-03,
			-3.7842324736531100e-03,
			-3.5338050023559580e-03,
			-3.1219319124799489e-03,
			-2.5864957712039751e-03,
			-1.9784179024470525e-03,
			-1.3569073131266510e-03,
			-7.8378825350351668e-04,
			-3.1744647022797834e-04,
			-6.9834312824419518e-06,
			1.1284000836526972e-04,
			2.5354317567758977e-05,
			-2.6597224788052265e-04,
			-7.3743067972836655e-04,
			-1.3469646033695993e-03,
			-2.0379172193921657e-03,
			-2.7443031575346403e-03,
			-3.3971190584163721e-03,
			-3.9310935787960494e-03,
			-4.2912209155740550e-03,
			-4.4384286657659696e-03,
			-4.3538016122489434e-03,
			-4.0409122987702514e-03,
			-3.5259856147050032e-03,
			-2.8558317578400881e-03,
			-2.0937002074628681e-03,
			-1.3134155200516908e-03,
			-5.9233317583970750e-04,
			-3.7822358250078755e-06,
			3.9027251319353107e-04,
			5.4562083721612718e-04,
			4.4059792201310989e-04,
			7.8766065900046770e-05,
			-5.1082710083981203e-04,
			-1.2758972595056852e-03,
			-2.1455177714411132e-03,
			-3.0366641508699954e-03,
			-3.8621118097987728e-03,
			-4.5389428080685424e-03,
			-4.9968394044701686e-03,
			-5.1853430528545691e-03,
			-5.0793389195515149e-03,
			-4.6821819764329082e-03,
			-4.0260977778495107e-03,
			-3.1697493995113059e-03,
			-2.1931374329631778e-03,
			-1.1902658708177875e-03,
			-2.6023694791261148e-04,
			5.0239086906258580e-04,
			1.0170537242174349e-03,
			1.2255086096815426e-03,
			1.0981873138275063e-03,
			6.3790127253392042e-04,
			-1.1953798113192900e-04,
			-1.1079276778061561e-03,
			-2.2363446488090528e-03,
			-3.3974065727959773e-03,
			-4.4773856919170718e-03,
			-5.3672354801872664e-03,
			-5.9734753110310982e-03,
			-6.2278624174949846e-03,
			-6.0948685932705384e-03,
			-5.5761654078248781e-03,
			-4.7115908003906980e-03,
			-3.5763979419758700e-03,
			-2.2749440304833169e-03,
			-9.3132842805940036e-04,
			3.2219796078291040e-04,
			1.3579883121752859e-03,
			2.0657109495232774e-03,
			2.3637734346849855e-03,
			2.2083038115951678e-03,
			1.5987342434481875e-03,
			5.7933196288304162e-04,
			-7.6360523631937476e-04,
			-2.3087604573219398e-03,
			-3.9104757284739783e-03,
			-5.4121833258849901e-03,
			-6.6613801997739783e-03,
			-7.5247452268142716e-03,
			-7.9019373222448622e-03,
			-7.7366891333259091e-03,
			-7.0240232005336425e-03,
			-5.8127492393513962e-03,
			-4.2028248373273342e-03,
			-2.3376400760274124e-03,
			-3.9177573876898449e-04,
			1.4447611912945188e-03,
			2.9844462893490794e-03,
			4.0606549183290512e-03,
			4.5448405291744071e-03,
			4.3607762636084981e-03,
			3.4943738979672815e-03,
			1.9979539455294236e-03,
			-1.1679981628951848e-05,
			-2.3614511802506899e-03,
			-4.8360047833993728e-03,
			-7.1964238219141821e-03,
			-9.2021151553450583e-03,
			-1.0633951165805386e-02,
			-1.1316552798255641e-02,
			-1.1137586000254470e-02,
			-1.0062124842053854e-02,
			-8.1405010867291669e-03,
			-5.5085851061377634e-03,
			-2.3800849646007840e-03,
			9.6884449252551230e-04,
			4.2216836898676243e-03,
			7.0486768194433849e-03,
			9.1368371170709086e-03,
			1.0220305897430315e-02,
			1.0108397670110335e-02,
			8.7087474354680741e-03,
			6.0432886744458819e-03,
			2.2553117568610858e-03,
			-2.3934561482566973e-03,
			-7.5360015862024794e-03,
			-1.2722801055507309e-02,
			-1.7452784005121087e-02,
			-2.1210303569516521e-02,
			-2.3505355684549270e-02,
			-2.3913919959109343e-02,
			-2.2115177846259819e-02,
			-1.7922511198655740e-02,
			-1.1305590976966528e-02,
			-2.4015034641907277e-03,
			8.4863183807199517e-03,
			2.0901652188791666e-02,
			3.4260844192471288e-02,
			4.7888335453839595e-02,
			6.1059894106108653e-02,
			7.3050446110118419e-02,
			8.3182990982319571e-02,
			9.0874957753215704e-02,
			9.5678513253330877e-02,
			9.7311769218428879e-02,
			9.5678513253330918e-02,
			9.0874957753215677e-02,
			8.3182990982319585e-02,
			7.3050446110118419e-02,
			6.1059894106108667e-02,
			4.7888335453839609e-02,
			3.4260844192471288e-02,
			2.0901652188791666e-02,
			8.4863183807199569e-03,
			-2.4015034641907285e-03,
			-1.1305590976966534e-02,
			-1.7922511198655757e-02,
			-2.2115177846259833e-02,
			-2.3913919959109346e-02,
			-2.3505355684549270e-02,
			-2.1210303569516531e-02,
			-1.7452784005121090e-02,
			-1.2722801055507311e-02,
			-7.5360015862024829e-03,
			-2.3934561482566981e-03,
			2.2553117568610867e-03,
			6.0432886744458819e-03,
			8.7087474354680741e-03,
			1.0108397670110337e-02,
			1.0220305897430322e-02,
			9.1368371170709086e-03,
			7.0486768194433849e-03,
			4.2216836898676261e-03,
			9.6884449252551273e-04,
			-2.3800849646007844e-03,
			-5.5085851061377642e-03,
			-8.1405010867291686e-03,
			-1.0062124842053856e-02,
			-1.1137586000254470e-02,
			-1.1316552798255644e-02,
			-1.0633951165805386e-02,
			-9.2021151553450635e-03,
			-7.1964238219141838e-03,
			-4.8360047833993763e-03,
			-2.3614511802506908e-03,
			-1.1679981628951848e-05,
			1.9979539455294258e-03,
			3.4943738979672854e-03,
			4.3607762636085007e-03,
			4.5448405291744088e-03,
			4.0606549183290546e-03,
			2.9844462893490803e-03,
			1.4447611912945196e-03,
			-3.9177573876898449e-04,
			-2.3376400760274133e-03,
			-4.2028248373273359e-03,
			-5.8127492393514005e-03,
			-7.0240232005336460e-03,
			-7.7366891333259160e-03,
			-7.9019373222448709e-03,
			-7.5247452268142768e-03,
			-6.6613801997739835e-03,
			-5.4121833258849944e-03,
			-3.9104757284739800e-03,
			-2.3087604573219420e-03,
			-7.6360523631937541e-04,
			5.7933196288304206e-04,
			1.5987342434481888e-03,
			2.2083038115951695e-03,
			2.3637734346849881e-03,
			2.0657109495232787e-03,
			1.3579883121752868e-03,
			3.2219796078291127e-04,
			-9.3132842805940057e-04,
			-2.2749440304833191e-03,
			-3.5763979419758734e-03,
			-4.7115908003907024e-03,
			-5.5761654078248851e-03,
			-6.0948685932705410e-03,
			-6.2278624174949872e-03,
			-5.9734753110311043e-03,
			-5.3672354801872690e-03,
			-4.4773856919170770e-03,
			-3.3974065727959803e-03,
			-2.2363446488090563e-03,
			-1.1079276778061574e-03,
			-1.1953798113192943e-04,
			6.3790127253392085e-04,
			1.0981873138275076e-03,
			1.2255086096815435e-03,
			1.0170537242174358e-03,
			5.0239086906258667e-04,
			-2.6023694791261213e-04,
			-1.1902658708177892e-03,
			-2.1931374329631804e-03,
			-3.1697493995113107e-03,
			-4.0260977778495150e-03,
			-4.6821819764329151e-03,
			-5.0793389195515218e-03,
			-5.1853430528545760e-03,
			-4.9968394044701747e-03,
			-4.5389428080685442e-03,
			-3.8621118097987776e-03,
			-3.0366641508699980e-03,
			-2.1455177714411145e-03,
			-1.2758972595056867e-03,
			-5.1082710083981311e-04,
			7.8766065900046770e-05,
			4.4059792201311075e-04,
			5.4562083721612805e-04,
			3.9027251319353150e-04,
			-3.7822358250078755e-06,
			-5.9233317583970880e-04,
			-1.3134155200516921e-03,
			-2.0937002074628711e-03,
			-2.8558317578400915e-03,
			-3.5259856147050088e-03,
			-4.0409122987702592e-03,
			-4.3538016122489503e-03,
			-4.4384286657659757e-03,
			-4.2912209155740620e-03,
			-3.9310935787960546e-03,
			-3.3971190584163777e-03,
			-2.7443031575346438e-03,
			-2.0379172193921687e-03,
			-1.3469646033696017e-03,
			-7.3743067972836828e-04,
			-2.6597224788052351e-04,
			2.5354317567759410e-05,
			1.1284000836526972e-04,
			-6.9834312824415181e-06,
			-3.1744647022797877e-04,
			-7.8378825350351841e-04,
			-1.3569073131266529e-03,
			-1.9784179024470564e-03,
			-2.5864957712039799e-03,
			-3.1219319124799550e-03,
			-3.5338050023559641e-03,
			-3.7842324736531187e-03,
			-3.8517604832979556e-03,
			-3.7330937104403505e-03,
			-3.4430325498542550e-03,
			-3.0126612251738143e-03,
			-2.4859984617802843e-03,
			-1.9154665600813439e-03,
			-1.3566415625985765e-03,
			-8.6280715728713248e-04,
			-4.7984317191924932e-04,
			-2.4193625100973301e-04,
			-1.6851079481957555e-04,
			-2.6265206842258801e-04,
			-5.1114349515264874e-04,
			-8.8608150223199096e-04,
			-1.3478794014496660e-03,
			-1.8493412139778472e-03,
			-2.3403892837598117e-03,
			-2.7729746824372626e-03,
			-3.1056912475649788e-03,
			-3.3076524683700315e-03,
			-3.3612706949638690e-03,
			-3.2636916931282319e-03,
			-3.0267727437604183e-03,
			-2.6756357782218901e-03,
			-2.2459644154696990e-03,
			-1.7803320420881675e-03,
			-1.3239361803047617e-03,
			-9.2016438994335268e-04,
			-6.0642476251474497e-04,
			-4.1063976990715395e-04,
			-3.4872998370800073e-04,
			-4.2331173523314271e-04,
			-6.2371066482157418e-04,
			-9.2726353456713318e-04,
			-1.3017563556190573e-03,
			-1.7087397545895596e-03,
			-2.1073826169179721e-03,
			-2.4584796038279785e-03,
			-2.7282208685875963e-03,
			-2.8913631482099622e-03,
			-2.9335066344339149e-03,
			-2.8522746288034630e-03,
			-2.6573034400949511e-03,
			-2.3690672229614721e-03,
			-2.0166749784740171e-03,
			-1.6348739179959453e-03,
			-1.2605657298138137e-03,
			-9.2918345122576030e-04,
			-6.7128325883948105e-04,
			-5.0967758766086110e-04,
			-4.5737696479821561e-04,
			-5.1652413990350054e-04,
			-6.7840413588374053e-04,
			-9.2450775236682593e-04,
			-1.2285242513423978e-03,
			-1.5590512379570643e-03,
			-1.8827443572853996e-03,
			-2.1675922672027411e-03,
			-2.3859964670479871e-03,
			-2.5173609120326171e-03,
			-2.5499498422015530e-03,
			-2.4818481466164642e-03,
			-2.3209490512706150e-03,
			-2.0839898846883422e-03,
			-1.7947487057919410e-03,
			-1.4815938115070857e-03,
			-1.1746371336738774e-03,
			-9.0277595818363940e-04,
			-6.9091251719947640e-04,
			-5.5761788704660311e-04,
			-5.1345808254887144e-04,
			-5.6013150133180361e-04,
			-6.9048502597345647e-04,
			-8.8938935754289588e-04,
			-1.1353710613629931e-03,
			-1.4028273596775388e-03,
			-1.6645966177239954e-03,
			-1.8946275302998434e-03,
			-2.0704856679088272e-03,
			-2.1754571889266573e-03,
			-2.2000535925879735e-03,
			-2.1427836247891172e-03,
			-2.0101324311648161e-03,
			-1.8157663436507688e-03,
			-1.5790565837738965e-03,
			-1.3230794514709379e-03,
			-1.0722982056842985e-03,
			-8.5015853938458566e-04,
			-6.7683314040934653e-04,
			-5.6733143777716502e-04,
			-5.3015062268610771e-04,
			-5.6658772472861503e-04,
			-6.7076573516148782e-04,
			-8.3035621871028087e-04,
			-1.0279134888687830e-03,
			-1.2426777301980277e-03,
			-1.4526618487227918e-03,
			-1.6368131495740745e-03,
			-1.7770380880220505e-03,
			-1.8598961554604264e-03,
			-1.8778052751213517e-03,
			-1.8296519661545317e-03,
			-1.7207597251464937e-03,
			-1.5622325114940253e-03,
			-1.3697506595220725e-03,
			-1.1619481531160521e-03,
			-9.5853814908442015e-04,
			-7.7837451944441616e-04,
			-6.3763933271861217e-04,
			-5.4832980445290724e-04,
			-5.1718531515799874e-04,
			-5.4514918710074623e-04,
			-6.2740578524895249e-04,
			-7.5397661608139343e-04,
			-9.1080504603679589e-04,
			-1.0812132551662208e-03,
			-1.2475813887483022e-03,
			-1.3930805785312590e-03,
			-1.5032900196379592e-03,
			-1.5675433814101875e-03,
			-1.5798796606311340e-03,
			-1.5395149164156405e-03,
			-1.4507998700714214e-03,
			-1.3226792145237412e-03,
			-1.1677166438897998e-03,
			-1.0007904473973103e-03,
			-8.3759419607065287e-04,
			-6.9309294300030650e-04,
			-5.8008622249482893e-04,
			-5.0801522925231191e-04,
			-4.8212458142003674e-04,
			-5.0305195443362970e-04,
			-5.6687547227462652e-04,
			-6.6560344061365078e-04,
			-7.8804826816350926e-04,
			-9.2099036038381839e-04,


			};
		if(*trig!=0)
			{
alg_locat();
			if(PGV_type==1)
				{
				datamid=0.0;
				now_fir=i_fir_list;
				for(int i=0;i<firLength;i++)
					{
					datamid=datamid+now_fir->dataV[4]*Fir[i];
					now_fir=now_fir->last;
					}
				i_fir_list->band_pass_dataV[4]=datamid;

alg_locat();

				datamid=0.0;
				now_fir=i_fir_list;
				for(int i=0;i<firLength;i++)
					{
					datamid=datamid+now_fir->dataD[4]*Fir[i];
					now_fir=now_fir->last;
					}
				i_fir_list->band_pass_dataD[4]=datamid;

				}
			else if(PGV_type==2)
				{
				for(int j=0;j<3;j++)
					{


					datamid=0.0;
					now_fir=i_fir_list;
					for(int i=0;i<firLength;i++)
						{
						datamid=datamid+now_fir->dataV[j]*Fir[i];
						now_fir=now_fir->last;
						}
					i_fir_list->band_pass_dataV[j]=datamid;

					}
				i_fir_list->band_pass_dataV[4]=pow((double)(i_fir_list->band_pass_dataV[0]*i_fir_list->band_pass_dataV[0]+i_fir_list->band_pass_dataV[1]*i_fir_list->band_pass_dataV[1]),0.5);
				for(int j=0;j<3;j++)
					{


					datamid=0.0;
					now_fir=i_fir_list;
					for(int i=0;i<firLength;i++)
						{
						datamid=datamid+now_fir->dataD[j]*Fir[i];
						now_fir=now_fir->last;
						}
					i_fir_list->band_pass_dataD[j]=datamid;
					}
				i_fir_list->band_pass_dataD[4]=pow((double)(i_fir_list->band_pass_dataD[0]*i_fir_list->band_pass_dataD[0]+i_fir_list->band_pass_dataD[1]*i_fir_list->band_pass_dataD[1]),0.5);
				}

			if(*trig==1)
				{
				for(jjj=0;jjj<3;jjj++)
					{
					band_pass_data->final_PGA_component[jjj]=abs(i_fir_list->band_pass_data[jjj]);
					band_pass_data->final_PGV_component[jjj]=abs(i_fir_list->band_pass_dataV[jjj]);
					band_pass_data->final_PGD_component[jjj]=abs(i_fir_list->band_pass_dataD[jjj]);
					}
				band_pass_data->final_PGA=i_fir_list->band_pass_data[j];
				band_pass_data->final_PGV=i_fir_list->band_pass_dataV[j];
				band_pass_data->final_PGD=i_fir_list->band_pass_dataD[j];
				*trig=2;
				*count+=1;
				}
			else
				{

alg_locat();
				for(jjj=0;jjj<3;jjj++)
					{
					if(abs(i_fir_list->band_pass_data[jjj])>band_pass_data->final_PGA_component[jjj])
						band_pass_data->final_PGA_component[jjj]=abs(i_fir_list->band_pass_data[jjj]);
					if(abs(i_fir_list->band_pass_dataV[jjj])>band_pass_data->final_PGV_component[jjj])
						band_pass_data->final_PGV_component[jjj]=abs(i_fir_list->band_pass_dataV[jjj]);
					if(abs(i_fir_list->band_pass_dataD[jjj])>band_pass_data->final_PGD_component[jjj])
						band_pass_data->final_PGD_component[jjj]=abs(i_fir_list->band_pass_dataD[jjj]);
					}

				if(i_fir_list->band_pass_data[4]>band_pass_data->final_PGA)
					{
					band_pass_data->final_PGA=i_fir_list->band_pass_data[4];
					band_pass_data->utc_second_final_PGA=i_fir_list->utc_second; //utc时间秒
					band_pass_data->utc_nanosecond_final_PGA=i_fir_list->utc_nanosecond; //utc时间纳秒

					}
				if(i_fir_list->band_pass_dataV[4]>band_pass_data->final_PGV)
					{
					band_pass_data->final_PGV=i_fir_list->band_pass_dataV[4];
					band_pass_data->utc_second_final_PGV=i_fir_list->utc_second; //utc时间秒
					band_pass_data->utc_nanosecond_final_PGV=i_fir_list->utc_nanosecond; //utc时间纳秒
					}
				if(i_fir_list->band_pass_dataD[4]>band_pass_data->final_PGD)
					{
					band_pass_data->final_PGD=i_fir_list->band_pass_dataD[4];
					band_pass_data->utc_second_final_PGD=i_fir_list->utc_second; //utc时间秒
					band_pass_data->utc_nanosecond_final_PGD=i_fir_list->utc_nanosecond; //utc时间纳秒
					}
				if(*count==timespan*200)

					{
					*count=1;
					return 2;
					}
				else
					{


					*count+=1;
					}



				}
			}
		return 1;
		}