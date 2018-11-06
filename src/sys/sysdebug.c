

int test_and_exit(int isexit)
{
	
	int i=5;
	printf("i=%lf\n",(double)i);

	char * file="/etc/app.conf";
	char * key="SN";
	char value[20];
	read_conf_value(key,value,file);
	printf("key=%s,value=%s\n",key,value);
	snprintf(value,"%s","SN:20170615");
	write_conf_value(key,value,file);
	read_conf_value(key,value,file);
	printf("key=%s,value=%s\n",key,value);
	exit(0);
}
