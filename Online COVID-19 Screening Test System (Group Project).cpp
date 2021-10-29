#include<iostream>
#include<string>
#include<vector>
#include<iomanip>
#include<cctype>
#include<cstdlib>
#include<fstream>

using namespace std; 
#define BASICCHARGE 300
#define HSEXTRA 50
const int NONLEAP[12]={31,28,31,30,31,30, 31, 31, 30, 31, 30, 31};
const int LEAP[12]={31,29,31,30,31,30, 31, 31, 30, 31, 30, 31};
const string HSSESSION[3]={"9.00am-12.00pm","1.00pm-4.00pm","4.00pm-7.00pm"};
const string DTSESSION[10]={"9.00am-10.00am","10.00am-11.00am","11.00am-12.00pm","12.00pm-1.00pm","1.00pm-2.00pm","2.00pm-3.00pm", "3.00pm-4.00pm","4.00pm-5.00pm","5.00pm-6.00pm","6.00-7.00"};
const string DAY[7]={"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
ofstream output,output2;
ifstream input;
fstream out;

class DateTime
{
	private:
		int dd;
		int day;
		int month;
		int year;
		int session;
	public:
		DateTime(int dd=0, int d=0, int m=0, int y=0, int s=0)
		{
			this->dd=dd;
			day=d;
			month=m;
			year=y;
			session=s;
		}
		void setDD(int d){dd=d;}
		void setSession(int s){session=s;}
		void setDay(int d){day=d;}
		void setMonth(int m){month=m;}
		void setYear(int y){year=y;}
		int getDD()const{return dd;}
		int getYear()const{return year;}
		int getMonth()const{return month;}
		int getDay()const{return day;}
		int getSession()const{return session;}
		void simplify(){
			if((year%4==0&&year%100!=0)||year%400)
			{
				if(day>0)
				{
					if(day>LEAP[month-1])
					{
						day-=LEAP[month-1];	
						month+=1;
					}	
				}
				else
				{
					if(month==1)
					{
						month=12;
						year-=1;
						day=LEAP[month-1]-abs(day);
					}
					else
					{
						day=LEAP[month-2]-abs(day);
						month-=1;
					}
				}
			}
			else 
			{
				if(day>0)
				{
					if(day>NONLEAP[month-1])
					{
						day-=NONLEAP[month-1];	
						month+=1;
					}	
				}
				else
				{
					if(month==1)
					{
						month=12;
						year-=1;
						day=NONLEAP[month-1]-abs(day);
					}
					else
					{
						day=NONLEAP[month-2]-abs(day);
						month-=1;
					}
				}
			}	
			if(month>12)
			{
				year+=1;
				month-=12;
			}
		}	
		string addDate(int d)
		{
			DateTime temp(dd, day,month, year, session);
			temp.day+=d;
			temp.simplify();
			return temp.getDate();
		}
		string getDate()const
		{
			string tempD, tempMM, tempYYYY;
			if(day<10)
				tempD="0"+to_string(day);
			else
				tempD=to_string(day);
			if(month<10)
				tempMM="0"+to_string(month);
			else
				tempMM=to_string(month);
			tempYYYY=to_string(year);
			return tempD+"/"+tempMM+"/"+tempYYYY;
		}
		int date_to_day(int d, int m, int y)
		{
			int tempDD;
			int diff_in_day;
			if(m>month)
			{
				if((y%4==0&&y%100!=0)||y%400==0)
					diff_in_day=(d+LEAP[m-2])-day;
				else
					diff_in_day=(d+NONLEAP[m-2])-day;
			}
			else if(month==12&&m==1)
				diff_in_day=(d+31)-day;
			else if(m==month)
				diff_in_day=d-day;
			
			tempDD=dd+diff_in_day;
			if(tempDD>6)
			{
				tempDD-=7;
			}
			return tempDD;	
		}	
		bool validDate(int d, int m, int y)
		{
			if(y<2020)
				return false;
			if(m<1||m>12)
				return false;
			if((y%4==0&&y%100!=0)||y%400==0)
			{
				if(d<1||d>LEAP[m-1])
					return false;
			}
			else
			{
				if(d<1||d>NONLEAP[m-1])
					return false;
			}
			return true;	
		}
		bool sevenDay(int d, int m, int y)
		{
			if(y<year||y>year+1)
				return false;
			if(y==year)
			{
				if(m<month||m>month+1)
					return false;
				if((y%4==0&&y%100!=0)||y%400==0)
				{
					if(m>month)
					{
						if(((d+LEAP[month-2])-day)>6)
							return false;
					}
					else if(m==month)
					{
						if((d-day)>6||d<day)
							return false;
					}	
				}
				else
				{
					if(m>month)
					{
						if(((d+NONLEAP[month-2])-day)>6)
							return false;
					}
					else if(m==month)
					{
						if((d-day)>6||d<day)
							return false;
					}	
				}	
			}
			else
			{ 
				if(month!=12||m!=1)
					return false;
				if(((d+31)-day)>6)
					return false;	
			}
			return true;
		}
		//For patient
		bool notToday(int d)
		{
			if(d==day)
				return false;
			else
				return true;
		}	
		bool operator<(const DateTime& date)
		{
			if(year<date.year)
				return true;
			else if(month<date.month)
				return true;
			else if(day<date.day)
				return true;
			else 
				return false;
		}
		bool operator==(const DateTime& date)
		{
			if(getDate()==date.getDate())
				return true;
			else 
				return false;
		}
};


class Charge{
	private:
		double cost;
	public:
		Charge(double c=0){cost=c;}
		void setCost(double c){cost=c;}
		void addCost(double c){cost+=c;}
		double getCost()const{return cost;}
		void clearPayment(){cost=0;	}
};
class Report{
	private:
		string result;
	public:
		Report(string r="None"){result=r;}
		void setResult(string r){result=r;}
		string getResult()const{return result;}
};

class Patient{
	protected:
		string name;
		string phone;
		string address;
		string ic;
		int age;
		string gender;
		Charge charge;
		DateTime* dt;
		Report* testResult;
	public:
		//Default constructor 
		Patient(string ic="",string n="", string p="",string ad="", int dd=0, int day=0,int month=0, int year=0, int time=0,int a=0, string g="", double c=0):charge(c){
			name=n;
			phone=p;
			address=ad;
			age=a;
			gender=g;
			this->ic=ic;
			dt=new DateTime(dd, day, month, year, time);
			testResult=new Report;
		}
		//Set name of patient
		void setName(string n){name=n;	}
		//Set phone of patient
		void setPhone(string p){phone=p;}
		//Set address of patient
		void setAddress(string ad){address=ad;}
		void setAge(int a){age=a;}
		void setGender(string g){gender=g;}
		void setIC(string ic){this->ic=ic;}
		void setReport(string r){testResult->setResult(r);}
		string getName()const{return name;}
		string getPhone()const{return phone;}
		string getAddress()const{return address;}
		int getAge()const{return age;}
		string getGender()const{return gender;}
		double getCharge()const{return charge.getCost();}
		string getIC()const{return ic;}
		string getTestResult()const{return testResult->getResult();}
		int getDay()const{return dt->getDay();	}
		int getMonth()const{return dt->getMonth();	}
		int getYear()const{return dt->getYear(); }
		int getDD()const{return dt->getDD();}
		int getSession()const{return dt->getSession();}
		string getDate()const{return dt->getDate();}
		virtual void displayData()const
		{
			cout<<left<<setw(15)<<"IC"<<setw(70)<<"Name"<<setw(10)<<"Age"<<setw(10)<<"Gender"<<setw(15)<<"Phone"<<setw(175)<<"Address"<<endl;
			cout<<setw(15)<<ic<<setw(70)<<name<<setw(10)<<age<<setw(10)<<gender<<setw(15)<<phone<<setw(175)<<address<<endl;
			cout<<"Result: "<<getTestResult()<<endl;
			cout<<"Date of appointment: "<<getDate()<<endl;
			cout<<"Time of appointment: Session "<<getSession()<<endl;
		}
		virtual void operator=(const Patient & p){}
		virtual void makePayment(){}
		virtual char getType()const{}
		virtual string getPlate()const{}
		virtual double getExtraFee()const{}	
		//Get all details of patients
		string dataString()
		{
			string c=to_string(charge.getCost());
			return ic+"\n"+name+"\n"+to_string(age)+"\n"+gender+"\n"+phone+"\n"+address+"\n"
			+c.substr(0,c.find('.')+3)+"\n"+getTestResult()+"\n"+getDate()+"\n"+to_string(getSession())+"\n";
		}
		friend class MedicalCentre; //does it necessary to use this
};

class HomeScreenPatient: public Patient
{
	protected:
		double extraFee;
		char type;
	public:
		HomeScreenPatient(string ic="",string n="", string p="",string ad="", int dd=0,int day=0, int month=0, int year=0, int session=0,int a=0, string g="", double c=0, double e=0)
		:Patient(ic, n,p,ad,dd,day,month,year,session,a,g,c)
		{
			extraFee=e;	
			type='H';
		}
		virtual void makePayment()
		{
			if(charge.getCost()==0)
			{
				cout<<"Payment is done already."<<endl;
			}
			else
			{
				cout<<"Bill:\nBasic charge per person: RM"<<BASICCHARGE<<endl;
				cout<<"Extra Service Charge is RM"<<HSEXTRA<<" per house which is only charged to the first person who registers for home-screening test."<<endl;
				cout<<"No addtional charge will be applied."<<endl;
				cout<<"Extra charge: RM"<<extraFee<<endl;
				cout<<"Total amount: RM"<<charge.getCost()<<endl;
				charge.clearPayment();
				cout<<"Payment is done.\nThank you."<<endl;
			}
		}
		virtual double getExtraFee()const{return extraFee;}	
		virtual void displayData()const
		{
			cout<<"Type of patient: Home Screening Patient"<<endl;
			Patient::displayData();
			cout<<"("<<HSSESSION[dt->getSession()-1]<<")"<<endl;
			cout<<"Bill:\nBasic charge per person: RM"<<BASICCHARGE<<endl;
			cout<<"Extra Service Charge is RM"<<HSEXTRA<<" per house which is only charged to the first person who registers for home-screening test."<<endl;
			cout<<"No addtional charge will be applied."<<endl;
			cout<<"Extra charge: RM"<<extraFee<<endl;
			cout<<"Total amount: RM"<<charge.getCost()<<endl;
		}
		virtual char getType()const{return type;}
		virtual void operator=(const Patient & p)
		{
			name=p.getName();
			phone=p.getPhone();
			address=p.getAddress();
			ic=p.getIC();
			age=p.getAge();
			gender=p.getGender();
			charge=p.getCharge();
			dt=new DateTime(p.getDD(), p.getDay(), p.getMonth(), p.getYear(), p.getSession());
			testResult=new Report(p.getTestResult());
			type='H';
			extraFee=p.getExtraFee();
		}
		
		friend class MedicalCentre;	
};

class DriveThruPatient: public Patient
{
	protected:
		string plateNumber;
		char type;
	public:
		DriveThruPatient(string ic="",string n="", string p="",string ad="", int dd=0, int day=0, int month=0, int year=0, int session=0, string plate="", int a=0, string g="", double c=0)
		:Patient(ic, n,p,ad,dd,day,month, year, session,a,g,c)
		{
			plateNumber=plate;	
			type='D';
		}
		virtual void makePayment()
		{
			if(charge.getCost()==0)
			{
				cout<<"Payment is done already."<<endl;
			}
			else
			{
				cout<<"Bill:\nBasic charge per person: RM"<<BASICCHARGE<<endl;
				cout<<"No addtional charge will be applied."<<endl;
				cout<<"Total amount: RM"<<charge.getCost()<<endl;
				charge.clearPayment();
				cout<<"Payment is done.\nThank you."<<endl;
			}
		}
		virtual void displayData()const
		{
			cout<<"Type of patient: Drive-Thru Patient"<<endl;
			Patient::displayData();
			cout<<"("<<DTSESSION[dt->getSession()-9]<<")"<<endl;
			cout<<"Bill:\nBasic charge per person: RM"<<BASICCHARGE<<endl;
			cout<<"Total amount: RM"<<charge.getCost()<<endl;
		}
		virtual char getType()const{return type;}
		virtual string getPlate()const{return plateNumber;}
		virtual void operator=(const Patient & p)//logic of overloaded = operator here , need to use patient itself or only hsp/dtp
		{
			name=p.getName();
			phone=p.getPhone();
			address=p.getAddress();
			ic=p.getIC();
			age=p.getAge();
			gender=p.getGender();
			charge=p.getCharge();
			dt=new DateTime(p.getDD(), p.getDay(), p.getMonth(), p.getYear(), p.getSession());
			testResult=new Report(p.getTestResult());
			type='D';
			plateNumber=p.getPlate();
		}
		friend class MedicalCentre;
};


class PosPatientHS:public HomeScreenPatient
{
	private:
		bool contacted;
	public:
		PosPatientHS(string ic="",string n="", string p="",string ad="", int dd=0,int day=0, int month=0, int year=0, int session=0,int a=0, string g="", double c=0, double e=0)
		:HomeScreenPatient(ic, n, p, ad, dd, day, month, year, session, a, g, c, e)
		{
			contacted=false;
		}
		void printReport()
		{
			out.open("PositivePatientList(HS).txt", ios::app); 
			out<<getDate()<<endl;
			out<<dataString();
			if(contacted)
				out<<"Contacted"<<endl<<endl;
			else
				out<<"Not contacted"<<endl<<endl;
			out.close();
		}
		void contactPatient()
		{
			char done;
			cout<<getPhone()<<endl;
			cout<<"Is the patient contacted? (Enter Y if yes and N if no)"<<endl;
			cin>>done;
			while(toupper(done)!='Y'&&toupper(done)!='N')
			{
				cout<<"Enter Y if Yes and N if No: "<<endl;
				cin>>done;
			}
			cin.ignore();
			if(done=='Y')
				contacted=true;
			else
				contacted=false;	
		}			
};

class PosPatientDT:public DriveThruPatient
{
	private:
		bool contacted;	
	public:
		PosPatientDT(string ic="",string n="", string p="",string ad="", int dd=0, int day=0, int month=0, int year=0, int session=0, string plate="", int a=0, string g="", double c=0)
		:DriveThruPatient(ic, n, p, ad, dd, day, month, year, session, plate, a, g, c)
		{
			contacted=false;
		}
		void printReport()
		{
			out.open("PositivePatientList(DT).txt", ios::app); 
			out<<getDate()<<endl;
			out<<dataString();
			if(contacted)
				out<<"Contacted"<<endl<<endl;
			else
				out<<"Not contacted"<<endl<<endl;
			out.close();
		}
		void contactPatient()
		{
			char done;
			cout<<getPhone()<<endl;
			cout<<"Is the patient contacted? (Enter Y if yes and N if no)"<<endl;
			cin>>done;
			while(toupper(done)!='Y'&&toupper(done)!='N')
			{
				cout<<"Enter Y if Yes and N if No: "<<endl;
				cin>>done;
			}
			cin.ignore();
			if(done=='Y')
				contacted=true;
			else
				contacted=false;	
		}			
};

//Abstract Base Class
class Appointment
{
	protected:
		DateTime* today;
	public:
		Appointment(int dd=0, int day=0, int month=0, int year=0)
		{
			today=new DateTime(dd, day, month, year,0);
		}
		virtual void getTimeTable()=0;
		void setDay(int d){today->setDay(d);}	
		void setMonth(int m){today->setMonth(m);}
		void setYear(int y){today->setYear(y);}
		void setDD(int d){today->setDD(d);}
		int getDay()const{return today->getDay();}
		int getMonth()const{return today->getMonth();}
		int getYear()const{return today->getYear();}
		int getDD()const{return today->getDD();}
		friend class MedicalCentre;
};
class HSAppointment:public Appointment
{
	private:
		string occupied[7][3][15];
	public:
		HSAppointment(int dd=0, int day=0, int month=0, int year=0):Appointment(dd,day, month, year)
		{
			for(int i=0; i<7; i++)
				for(int j=0; j<3; j++)
					for(int k=0; k<15; k++)
						occupied[i][j][k].clear();
		}
		virtual void getTimeTable(){
			int d, diff;
			DateTime temp(0, 0,getMonth(),getYear());
			cout<<"Booking made for 6 days starting from tomorrow"<<endl;
			cout<<"Session 1(9.00am), Session 2(1.00pm), Session 3(3.00pm)"<<endl;
			cout<<"* represents available slot, X represents occupied slot."<<endl;
			cout<<left<<setw(14)<<"Session"<<"1 2 3"<<endl;
			for(int i=0; i<7; i++)
			{
				if(i==getDD())
					cout<<setw(14)<<today->getDate();
				else if(i<getDD())
				{
					diff=getDD()-i;
					d=getDay()+(7-diff);
					temp.setDay(d);
					temp.simplify();
					cout<<setw(14)<<temp.getDate();
				}
				else
				{
					diff=i-getDD();
					d=getDay()+diff;
					temp.setDay(d);
					temp.simplify();
					cout<<setw(14)<<temp.getDate();
				}
				for(int j=0; j<3; j++)
				{
					if(occupied[i][j][0]!="")
						cout<<"X ";
					else
						cout<<"* ";
				}
				cout<<DAY[i]<<endl;}
		}
		bool check_if_occupied(int day, int month, int year, int time)
		{
			int d= today->date_to_day(day, month, year);
			if(occupied[d][time-1][0]!="")
			{
				cout<<"This timeslot is full"<<endl;
				return true;
			}
			else
				return false;				
		}
		void makeAppointment(string ic, int day,int month, int year, int t)
		{
			int d= today->date_to_day(day, month, year);
			for(int i=0; i<15; i++)
			{
				if(occupied[d][t-1][i].empty())
				{	
					occupied[d][t-1][i]=ic;
					break;	
				}
			}
		}
		friend class MedicalCentre;
};
class DTAppointment: public Appointment
{
	private:
		string occupied[7][10][5];
	public:
		DTAppointment(int dd=0, int day=0, int month=0, int year=0):Appointment(dd, day, month, year)
		{
			for(int i=0; i<7; i++)
				for(int j=0; j<10; j++)
					for(int k=0; k<5; k++)
						occupied[i][j][k].clear();
		}
		virtual void getTimeTable()
		{
			int d, diff;
			DateTime temp(0, 0, getMonth(), getYear());
			cout<<"Booking made for 6 days starting from tomorrow"<<endl;
			cout<<"Each session is 1 hour."<<endl;
			cout<<"* represents available slot, X represents occupied slot."<<endl;
			cout<<left<<setw(20)<<"24-hours format";
			cout<<setw(8)<<"09:00"<<setw(8)<<"10:00"<<setw(8)<<"11:00"<<setw(8)<<"12:00"<<setw(8)
			<<"13:00"<<setw(8)<<"14:00"<<setw(8)<<"15:00"<<setw(8)<<"16:00"<<setw(8)<<"17:00"<<setw(8)<<"18:00"<<endl;
			for(int i=0; i<7; i++)
			{
				if(i==getDD())
					cout<<setw(20)<<today->getDate();
				else if(i<getDD())
				{
					diff=getDD()-i;
					d=getDay()+(7-diff);
					temp.setDay(d);
					temp.simplify();
					cout<<setw(20)<<temp.getDate();
				}
				else
				{
					diff=i-getDD();
					d=getDay()+diff;
					temp.setDay(d);
					temp.simplify();
					cout<<setw(20)<<temp.getDate();
				}
				for(int j=0; j<10; j++)
				{
					for(int k=0; k<5; k++)
					{
						if(occupied[i][j][k]!="")
							cout<<"X";
						else
							cout<<"*";
					} 
					cout<<"   ";
				}
				cout<<DAY[i]<<endl;
			}
		}		
		bool check_if_occupied(int day, int month, int year, int time)
		{
			int d= today->date_to_day(day, month, year);
			bool occupy=true;
			int i=0;
			while(i<5&&occupy==true)
			{
				if(occupied[d][time-9][i].empty())
					occupy=false;	
					i++;
			}
			if(occupy==true)
			{
				cout<<"This timeslot is full"<<endl;
			}
			return occupy;				
		}
		void makeAppointment(string ic, int day,int month, int year, int t)
		{
			int d= today->date_to_day(day, month, year);
			for(int i=0; i<5; i++)
			{
				if(occupied[d][t-9][i].empty())
				{
					occupied[d][t-9][i]=ic;
					break;	
				}
			}	
		}
		friend class MedicalCentre;
};

class MedicalCentre
{
	private:
		vector<Patient*> patientList;
		DTAppointment dThru;
		HSAppointment hScreen;
	public:
		MedicalCentre(int dd=0, int day=0, int month=0, int year=0):dThru(dd,day,month,year), hScreen(dd,day,month,year){}
		//add if no record ,print no record
		void checkDTAppointment(int day, int month, int year, int t)
		{ 
			int d=dThru.today->date_to_day(day, month, year);
			for(int j=0;dThru.occupied[d][t-9][j]!="";j++)
			{
				cout<<"\nPatient "<<j+1<<endl;
				printData(dThru.occupied[d][t-9][j]);	
			}
		}
		//add if no record ,print no record
		void checkHSAppointment(int day, int month, int year, int t)
		{ 
			int d=hScreen.today->date_to_day(day, month, year);
			for(int j=0;hScreen.occupied[d][t-1][j]!="";j++ )
			{
				cout<<"\nPatient "<<j+1<<endl;
				printData(hScreen.occupied[d][t-1][j]);
			}
		}
		void setResult(string ic, string r)
		{
			for(int i=0; i<patientList.size();i++)
			{
				if(patientList[i]->getIC()==ic)
				{
					patientList[i]->setReport(r);
					break;		
				}
			}	
			saveData();
		}
		//add if patientlist is empty, print no record yet
		bool checkIC(string ic)
		{    
			bool found=false;
			int i=0;
			while(found==false&&i<patientList.size())
			{
				if(patientList[i]->getIC()==ic)
				found=true;
				i++;
			}
			return found;	
		}
		void printData(string ic)const
		{
			for(int i=0; i<patientList.size(); i++)
			{
				if(patientList[i]->getIC()==ic)
				{
					patientList[i]->displayData();
					break;
				}
			}
		}
		string getData(string ic)
		{
			for(int i=0; i<patientList.size(); i++)
			{
				if(patientList[i]->getIC()==ic)
					return patientList[i]->dataString();	
			}
		}
		void managePosPatient(string ic)const
		{
			char type;
			for(int i=0; i<patientList.size(); i++)
			{
				if(patientList[i]->getIC()==ic)
				{
					type=patientList[i]->getType();
					if(type=='H')
					{
						PosPatientHS temp(patientList[i]->getIC(),patientList[i]->getName(), patientList[i]->getPhone(), patientList[i]->getAddress(), patientList[i]->getDD(), patientList[i]->getDay(), patientList[i]->getMonth(), patientList[i]->getYear(), patientList[i]->getSession(),patientList[i]->getAge(), patientList[i]->getGender(), patientList[i]->getCharge(), patientList[i]->getExtraFee());
						temp.contactPatient();
						temp.printReport();
					}
					else
					{
						PosPatientDT temp(patientList[i]->getIC(),patientList[i]->getName(), patientList[i]->getPhone(), patientList[i]->getAddress(), patientList[i]->getDD(), patientList[i]->getDay(), patientList[i]->getMonth(), patientList[i]->getYear(), patientList[i]->getSession(), patientList[i]->getPlate(), patientList[i]->getAge(), patientList[i]->getGender(), patientList[i]->getCharge());
						temp.contactPatient();
						temp.printReport();
					}
					break;	
				}
			}
		}
		void patientMakePayment(string ic)
		{
			for(int i=0; i<patientList.size(); i++)
			{
				if(patientList[i]->getIC()==ic)
				{
					patientList[i]->makePayment();
					break;
				}
			}
			saveData();
		}
		void updateApp()
		{
			//add one day to hsappointment's today's date
			DateTime temp(hScreen.getDD(), hScreen.getDay()+1, hScreen.getMonth(), hScreen.getYear());
			//modify to correct date
			temp.simplify();
			//check 0-6 day
			int d=hScreen.today->date_to_day(temp.getDay(),temp.getMonth(),temp.getYear());
			//automatically set today's date
			hScreen.setDay(temp.getDay());
			hScreen.setMonth(temp.getMonth());
			hScreen.setYear(temp.getYear());
			hScreen.setDD(d);
			dThru.setDay(temp.getDay());
			dThru.setMonth(temp.getMonth());
			dThru.setYear(temp.getYear());
			dThru.setDD(d);
			//clear yesterday's record
			for(int t=0;t<10; t++)
			{
				for(int j=0;j<5;j++)
				{
					if(d==0)
						(dThru.occupied[d+6][t][j]).clear();
					else
						(dThru.occupied[d-1][t][j]).clear();
				}
			}
			for(int t=0; t<3; t++)
			{
				for(int j=0;j<15;j++ )
				{
					if(d==0)
						(hScreen.occupied[d+6][t][j]).clear();
					else
						(hScreen.occupied[d-1][t][j]).clear();	
				}
			}
			saveData();
		}
		void registerHST()
		{
			//string n;
			int day, month, year, session, age, numPerson, choice;
			double extra,total;
			string name, phone, address, gender, ic;
			bool valid;
			cout<<"\nEnter the number of people to receive the test: (max 15)";
			cin>>numPerson;
			//numPerson=stoi(n);
			while (numPerson>15||numPerson<1)
			{
				cout<<"Max 15 people.\nEnter the number of people to receive the test.(1~15)"<<endl;
				cin>>numPerson;
				//numPerson=stoi(n);	
		    }
			hScreen.getTimeTable();
			cout<<"\nContinue? (Enter (1:continue, 0:exit)";
			cin>>choice;
			while(choice!=0&&choice!=1)
			{
				cout<<"\nContinue? (Enter (1:continue, 0:exit)";
				cin>>choice;
			}
			//return to main 
			if(choice==0)
				return;
			do{
				//enter appointment's date 
				cout<<"Enter date(DD/MM/YY): "<<endl;
				cout<<"Day(DD): ";
				cin>>day;
				cout<<"\nMonth(MM): ";
				cin>>month;
				cout<<"\nYear(YY): ";
				cin>>year;
				if((hScreen.today)->validDate(day, month, year)==false)
					valid=false;
				else if ((hScreen.today)->sevenDay(day, month, year)==false)
					valid=false;
				else if((hScreen.today)->notToday(day)==false)	// if today, valid=false
					valid=false;
				else 
					valid=true;
				while(valid==false)
				{
					cout<<"The date entered is wrong. The date must be in correct format and within 6 days from tomorrow."<<endl;
					cout<<"Appointment cannot be made for today."<<endl;
					cout<<"Enter date(DD/MM/YY): "<<endl;
					cout<<"Day(DD): ";
					cin>>day;
					cout<<"\nMonth(MM): ";
					cin>>month;
					cout<<"\nYear(YY): ";
					cin>>year;
				if((hScreen.today)->validDate(day, month, year)==false)
					valid=false;
				else if ((hScreen.today)->sevenDay(day, month, year)==false)
					valid=false;
				else if((hScreen.today)->notToday(day)==false)
					valid=false;
				else 
					valid=true;	
				}
				cout<<"\nEnter session: (Enter 1 (9.00am), 2 (1.00pm), 3 (3.00pm))\nThe duration of each session is 3 hours.";
				cin>>session;
				while(session<1||session>3)
				{
					cout<<"\nEnter 1~3: ";
					cin>>session;
				}
			}while(hScreen.check_if_occupied(day, month, year, session));
			cin.ignore();	
			cout<<"\nRegistration: ";
			for(int i=0; i<numPerson; i++)
			{
				cout<<"\nPerson "<<i+1<<endl;
				cout<<"Your IC number is :";
				getline(cin, ic);
				cout<<"\nEnter fullname (on IC):";
				getline(cin, name);
				cout<<"\nEnter phone number:(01X-XXXXXXXX/07-XXXXXXX)";
				getline(cin, phone);
				cout<<"\nEnter full address: ";
				getline(cin, address);
				cout<<"\nEnter gender: (Male/Female)";
				getline(cin, gender);
				while(gender!="Male"&&gender!="Female")
				{
					cout<<"\nPlease enter gender in correct form (Male/Female):";
					getline(cin, gender);
				}
				cout<<"\nEnter Age:";
				cin>>age;
				cin.ignore();
				if(i==0)
					extra=HSEXTRA;
				else
					extra=0;
				total=BASICCHARGE+extra;
				patientList.push_back(new HomeScreenPatient(ic, name, phone, address, (hScreen.today)->date_to_day(day, month, year),day, month, year,session, age, gender, total, extra));
				hScreen.makeAppointment(ic, day, month, year, session);
			}
			saveData();	
		}
		void  registerDTT()
		{
			int choice, day, month, session,year, age;
			string ic, name, phone, address, plate, gender;
			bool valid;
			dThru.getTimeTable();
			cout<<"\nContinue? (Enter (1:continue, 0:exit)";
			cin>>choice;
			while(choice!=0&&choice!=1)
			{
				cout<<"\nContinue? (Enter (1:continue, 0:exit)";
				cin>>choice;
			}
			if(choice==0)
				return;
			do{
				cout<<"Enter date(DD/MM/YY): "<<endl;
				cout<<"Day(DD): ";
				cin>>day;
				cout<<"\nMonth(MM): ";
				cin>>month;
				cout<<"\nYear(YY): ";
				cin>>year;
				if((dThru.today)->validDate(day, month, year)==false)
					valid=false;
				else if ((dThru.today)->sevenDay(day, month, year)==false)
					valid=false;
				else if((dThru.today)->notToday(day)==false)
					valid=false;
				else 
					valid=true;
				while(valid==false)
				{
					cout<<"The date entered is wrong. The date must be in correct format and within 6 days from tomorrow."<<endl;
					cout<<"Appointment cannot be made for today."<<endl;
					cout<<"Enter date(DD/MM/YY): "<<endl;
					cout<<"Day(DD): ";
					cin>>day;
					cout<<"\nMonth(MM): ";
					cin>>month;
					cout<<"\nYear(YY): ";
					cin>>year;
					if((dThru.today)->validDate(day, month, year)==false)
						valid=false;
					else if ((dThru.today)->sevenDay(day, month, year)==false)
						valid=false;
					else if((dThru.today)->notToday(day)==false)
						valid=false;
					else 
						valid=true;	
				}
				cout<<"\nEnter session: (Enter 9~18). \nThe duration of each session is 1 hour."<<endl;
				cout<<"eg. 13 represents 13:00 - 14:00 (24-hour format)"<<endl;
				cin>>session;
				while(session<9||session>18)
				{
					cout<<"\nEnter 9~18: ";
					cin>>session;
				}
			}while(dThru.check_if_occupied(day, month, year,session));
			cin.ignore();	
			cout<<"\nRegistration: ";
			cout<<"\nYour IC number is ";
			getline(cin, ic);
			cout<<"\nEnter fullname (on IC):";
			getline(cin, name);
			cout<<"\nEnter phone number: (01X-XXXXXXXX/07-XXXXXXX)";
			getline(cin, phone);
			cout<<"\nEnter full address:";
			getline(cin, address);
			cout<<"\nEnter car plate:";
			getline(cin, plate);
			cout<<"\nEnter gender: (Male/Female)";
			getline(cin, gender);
			while(gender!="Male"&&gender!="Female")
			{
				cout<<"\nPlease enter gender in correct form (Male/Female):";
				getline(cin, gender);
			}
			cout<<"\nEnter Age:";
			cin>>age;
			dThru.makeAppointment(ic, day, month, year, session);
			patientList.push_back(new DriveThruPatient(ic, name, phone, address, (dThru.today)->date_to_day(day, month, year),day, month, year,session, plate, age, gender,BASICCHARGE));				
			saveData();
		}
		//renew data of patients when there is any change
		void saveData()
		{
			int startScan, minIndex, numHS=0, numDT=0;
			Patient* earliestHS=new HomeScreenPatient;
			Patient* earliestDT=new DriveThruPatient;
			vector<Patient*> h;
			vector<Patient*> d;
			string date;
			int session, numSession, numPatient;
			output.open("DTPatientList.txt");
			output2.open("HSPatientList.txt");
	
			for(int i=0; i<patientList.size(); i++)
			{
				if(patientList[i]->getType()=='H')
				{
					h.push_back(new HomeScreenPatient);
					//overloaded = operator in patient class
					*h[numHS] = *patientList[i];
					numHS++;
				}
				else
				{
					d.push_back(new DriveThruPatient);
					*d[numDT]=*patientList[i];
					numDT++;
				}	
			}
			//Sort vector h in ascending order of the date and session
			for(startScan=0; startScan<(h.size()-1); startScan++)
			{
				minIndex=startScan;
				*earliestHS=*h[startScan];
				for(int i=startScan+1; i<h.size();i++)
				{
					//overloaded < operator
					if(*(h[i]->dt)<*(earliestHS->dt))
					{
						minIndex=i;
						*earliestHS=*h[i];	
					}
					//overloaded == operator
					else if(*(h[i]->dt)==*(earliestHS->dt))
					{
						if(h[i]->getSession()<earliestHS->getSession())
						{
							minIndex=i;
							*earliestHS=*h[i];	
						}
					}
				}
				*h[minIndex]=*h[startScan];
				*h[startScan]=*earliestHS;
			}
			//sort vector d in ascending order of the date and session
			for(startScan=0; startScan<(d.size()-1); startScan++)
			{
				minIndex=startScan;
				*earliestDT=*d[startScan];
				for(int i=startScan+1; i<d.size();i++)
				{
					if(*(d[i]->dt)<*(earliestDT->dt))
					{
						minIndex=i;
						*earliestDT=*d[i];	
					}
					else if(*(d[i]->dt)==*(earliestDT->dt))
					{
						if(d[i]->getSession()<earliestDT->getSession())
						{
							minIndex=i;
							*earliestDT=*d[i];	
						}
					}
				}
				*d[minIndex]=*d[startScan];
				*d[startScan]=*earliestDT;
			}
			//first hs patients' list (appointment date)
			date=h[0]->getDate();
			output2<<"~ "<<date<<endl;
			session=h[0]->getSession();
			numSession=1;
			output2<<numSession<<") Session "<<session<<endl;
			numPatient=0;
			for(int i=0; i<h.size();i++)
			{
					if(h[i]->getDate()!=date)
					{
						date=h[i]->getDate();
						output2<<"~ "<<date<<endl;
						session=h[i]->getSession();
						numSession=1;
						output2<<numSession<<") Session "<<session<<endl;
						numPatient=1;
						output2<<"Patient "<<numPatient<<": "<<endl;
					}
					else if(h[i]->getSession()!=session)
					{
						session=h[i]->getSession();
						++numSession;
						output2<<numSession<<") Session"<<session<<endl;
						numPatient=1;
						output2<<"Patient "<<numPatient<<": "<<endl;
					}
					else
					{
						++numPatient;
						output2<<"Patient "<<numPatient<<": "<<endl;
					}
					output2<<h[i]->dataString()<<endl;	
			}
			//For dt patients' list
			output<<(dThru.today)->getDate()<<endl;
			output<<dThru.getDD()<<endl;
			date=d[0]->getDate();
			output<<"~ "<<date<<endl;
			session=d[0]->getSession();
			numSession=1;
			output<<numSession<<") Session "<<DTSESSION[session-9]<<endl;
			numPatient=0;
			for(int i=0; i<d.size();i++)
			{
					if(d[i]->getDate()!=date)
					{
						date=d[i]->getDate();
						output<<"~ "<<date<<endl;
						session=d[i]->getSession();
						numSession=1;
						output<<numSession<<") Session "<<DTSESSION[session-9]<<endl;
						numPatient=1;
						output<<"Patient "<<numPatient<<": "<<endl;
					}
					else if(d[i]->getSession()!=session)
					{
						session=d[i]->getSession();
						++numSession;
						output<<numSession<<") Session"<<DTSESSION[session-9]<<endl;
						numPatient=1;
						output<<"Patient "<<numPatient<<": "<<endl;
					}
					else
					{
						++numPatient;
						output<<"Patient "<<numPatient<<": "<<endl;
					}
					output<<d[i]->dataString();	
					output<<d[i]->getPlate()<<endl<<endl;
			}
			output.close();
			output2.close();
			delete earliestHS;
			delete earliestDT;
			for(int i=0; i<h.size();i++)
				delete h[i];
			for(int i=0; i<d.size(); i++)
				delete d[i];
		
		}
		void InsertHSList()
		{
			int day, month, year, session, age;
			double extra, charge;
			string name, phone, address, gender, ic, result;
			string in;
			input.open("HSPatientList.txt");
			if(!input)
			{
				cout<<"Error: Can't find the file.'"<<endl;
				exit(1);
			}
			getline(input, in);
			while(!input.eof())
			{
				if(in[0]=='P')
				{
					getline(input, in);
					ic=in;
					getline(input, in);
					name=in;
					getline(input, in);
					age=stoi(in);
					getline(input, in);
					gender=in;
					getline(input, in);
					phone=in;
					getline(input, in);
					address=in;
					getline(input, in);
					charge=stof(in);
					extra=stof(in)-BASICCHARGE;
					getline(input, in);
					result=in;
					getline(input, in);
					day=stoi(in.substr(0,2));
					in.erase(0,3);
					month=stoi(in.substr(0,2));
					in.erase(0,3);
					year=stoi(in);
					getline(input, in);
					session=stoi(in);
					patientList.push_back(new HomeScreenPatient(ic, name, phone, address, (hScreen.today)->date_to_day(day, month, year),  day, month, year,session, age, gender, charge, extra));
				 	getline(input, in);	
				 	if(checkSevenDay(day, month, year))
				 		hScreen.makeAppointment(ic, day, month, year, session);
				 }
		 		getline(input, in);
			}	
			input.close();
		}
		void InsertDTList()
		{
			int dd, day, month, year, session, age;
			string name, phone, address, gender, ic, result, plate;
			string in;
			input.open("DTPatientList.txt");
			if(!input)
			{
				cout<<"Error: Can't find the file.'"<<endl;
				exit(1);
			}
			getline(input, in);
			day=stoi(in.substr(0,2));
			in.erase(0,3);
			month=stoi(in.substr(0,2));
			in.erase(0,3);
			year=stoi(in);
			getline(input, in);
			dd=stoi(in);
			hScreen.setDay(day);
			hScreen.setMonth(month);
			hScreen.setYear(year);
			hScreen.setDD(dd);
			dThru.setDay(day);
			dThru.setMonth(month);
			dThru.setYear(year);
			dThru.setDD(dd);
			getline(input, in);
			while(!input.eof())
			{
				if(in[0]=='P')
				{
					getline(input, in);
					ic=in;
					getline(input, in);
					name=in;
					getline(input, in);
					age=stoi(in);
					getline(input, in);
					gender=in;
					getline(input, in);
					phone=in;
					getline(input, in);
					address=in;
					getline(input, in); //read charge (no use)
					getline(input, in);
					result=in;
					getline(input, in);
					day=stoi(in.substr(0,2));
					in.erase(0,3);
					month=stoi(in.substr(0,2));
					in.erase(0,3);
					year=stoi(in);
					getline(input, in);
					session=stoi(in);
					getline(input, in);
					plate=in;
					patientList.push_back(new DriveThruPatient(ic, name, phone, address, (dThru.today)->date_to_day(day, month, year),day, month, year,session, plate, age, gender,BASICCHARGE));
					getline(input, in);	
					if(checkSevenDay(day, month, year))
						dThru.makeAppointment(ic, day, month, year, session);	
				}
				getline(input, in);	
			}
			input.close();
		}
		bool checkValidDate(int day, int month, int year)
		{
			return (dThru.today)->validDate(day, month, year);
		}
		bool checkSevenDay(int day, int month, int year)
		{
			return (dThru.today)->sevenDay(day, month, year);
		}
		
};

int medMenu()
{
	int choice;
	cout<<"\n1 Check DriveThru Appointment"<<endl;
	cout<<"2 Check HomeScreening Appointment"<<endl;
	cout<<"3 Update Result"<<endl;
	cout<<"4 Search Patient"<<endl;
	cout<<"5 Daily Update"<<endl;
	cout<<"0 Exit"<<endl;
	cout<<"Enter 0~5:";
	cin>>choice;
	while(choice<0||choice>5)
	{
		cout<<"\nEnter 0~5:";
		cin>>choice;
	}
	return choice;
}

int patientMenu()
{
	int choice;
	cout<<"\n1 Register"<<endl;
	cout<<"2 Check Personal Detail, Result and Bill"<<endl;
	cout<<"3 Make payment"<<endl;
	cout<<"0 Exit"<<endl;
	cin>>choice;
	while(choice<0||choice>3)
	{
		cout<<"\nEnter 0~3:";
		cin>>choice;
	}
	return choice;	
}


int main()
{	
	int medChoice, patChoice, choice,dd, day, month, year, session;
	char identity;
	string password, ic, result;
	bool valid;
	cout<<"******< SMC MEDICAL CENTRE >******"<<endl;
	cout<<"Welcome ! (~ ^o^ ~)" <<endl;
	MedicalCentre SMC;
	SMC.InsertDTList();
	SMC.InsertHSList();
	do{
	cout<<"\nMedical Centre Staff Or Patient?: (Enter M for the former and P for the latter)";
	cin>>identity;
	while(toupper(identity)!='M'&&toupper(identity)!='P')
	{
		cout<<"\nMedical Centre Staff Or Patient?: (Enter M for the former and P for the latter)";
		cin>>identity;
	}
	cin.ignore();
	if(toupper(identity)=='M')
	{
		cout<<"\nPlease type in the password:";
		getline(cin, password);
		while(password!="325301")
		{
			cout<<"\nPassword Wrong. Please retype password: ";
			getline(cin,password);
		}
		medChoice=medMenu();
		switch(medChoice)
		{
				//check appointment date within seven days 
		case 1: cout<<"Enter date(DD/MM/YY): "<<endl;
				cout<<"Day(DD): ";
				cin>>day;
				cout<<"\nMonth(MM): ";
				cin>>month;
				cout<<"\nYear(YY): ";
				cin>>year;
				if(SMC.checkValidDate(day, month, year)==false)
					valid=false;
				else if (SMC.checkSevenDay(day, month, year)==false)
					valid=false;
				else 
					valid=true;
				while(valid==false)
				{
					cout<<"The date entered is wrong. The date must be in correct format and within 7 days starting from today"<<endl;
					cout<<"Enter date(DD/MM/YY): "<<endl;
					cout<<"Day(DD): ";
					cin>>day;
					cout<<"\nMonth(MM): ";
					cin>>month;
					cout<<"\nYear(YY): ";
					cin>>year;
				if(SMC.checkValidDate(day, month, year)==false)
					valid=false;
				else if (SMC.checkSevenDay(day, month, year)==false)
					valid=false;
				else 
					valid=true;	
				}
				cout<<"\nEnter session:"<<endl;
				cin>>session;
				while(session<9||session>18)
				{
					cout<<"\nEnter 9~18: ";
					cin>>session;
				}
				SMC.checkDTAppointment(day, month, year, session);
				break;
		case 2: cout<<"Enter date(DD/MM/YY): "<<endl;
				cout<<"Day(DD): ";
				cin>>day;
				cout<<"\nMonth(MM): ";
				cin>>month;
				cout<<"\nYear(YY): ";
				cin>>year;
				if(SMC.checkValidDate(day, month, year)==false)
					valid=false;
				else if (SMC.checkSevenDay(day, month, year)==false)
					valid=false;
				else 
					valid=true;
				while(valid==false)
				{
					cout<<"The date entered is wrong. The date must be in correct format and within 7 days starting from today"<<endl;
					cout<<"Enter date(DD/MM/YY): "<<endl;
					cout<<"Day(DD): ";
					cin>>day;
					cout<<"\nMonth(MM): ";
					cin>>month;
					cout<<"\nYear(YY): ";
					cin>>year;
				if(SMC.checkValidDate(day, month, year)==false)
					valid=false;
				else if (SMC.checkSevenDay(day, month, year)==false)
					valid=false;
				else 
					valid=true;	
				}
				cout<<"\nEnter session:";
				cin>>session;
				while(session<1||session>3)
				{
					cout<<"\nEnter 1~3: ";
					cin>>session;
				}
				SMC.checkHSAppointment(day,month, year, session);
				break;
				
				//Enter result(positive/negative)
		case 3: cout<<"\nEnter IC of patient:";
				cin>>ic;
				//******explain how to check IC
				while(!SMC.checkIC(ic))
				{
					cout<<"Wrong ic:"<<endl;
					cout<<"Enter IC of patient:";
					cin>>ic;
				}
				cout<<"Enter result: (Positive/Negative)"<<endl;
				cin>>result;
				while(result!="Positive"&&result!="Negative")
				{
					cout<<"Enter the correct result: "<<endl;
					cin>>result;
				}
				SMC.setResult(ic, result);
				if(result=="Positive")
					SMC.managePosPatient(ic);
				break;
		case 4: cout<<"\nEnter IC of patient:";
				cin>>ic;
				while(!SMC.checkIC(ic))
				{
					cout<<"\nWrong IC";
					cout<<"\nEnter IC of patient:";
					cin>>ic;
				}
				SMC.printData(ic);
				break;
		case 5: SMC.updateApp();
				break;
		case 0: cout<<"Return back."<<endl;
				break;
		}
	}
	else if(toupper(identity)=='P')
	{
		patChoice=patientMenu();
		switch(patChoice)
		{
			case 1: cout<<"\nHome-Screening Or Drive-Thru:? (Enter H for the former or D for the latter)";
					cin>>identity;
					while(toupper(identity)!='H'&&toupper(identity)!='D')
					{
						cout<<"\nHome-Screening Or Drive-Thru:? (Enter H for the former or D for the latter)";
						cin>>identity;
					}
					if(toupper(identity)=='H')
						SMC.registerHST();
					else
						SMC.registerDTT();
					break;
					//know personal details of yourself
			case 2: cout<<"\nEnter your IC:"<<endl;
					cin>>ic;
					while(!SMC.checkIC(ic))
					{
						cout<<"<Wrong IC>"<<endl;
						cout<<"Reenter your IC:"<<endl;
						cin>>ic;
					}
					SMC.printData(ic);
					break;
					//make payment 
			case 3: cout<<"\nEnter your IC:"<<endl;
					cin>>ic;
					while(!SMC.checkIC(ic))
					{
						cout<<"<Wrong IC>"<<endl;
						cout<<"Reenter your IC:";
						cin>>ic;
					}
					SMC.patientMakePayment(ic);
					break;
			case 4: cout<<"\nReturn back"<<endl;
		}
	}
	cout<<"Do you want to shutdown this application? (1= yes, 0= no)"<<endl;
	cin>>choice;
	while(choice!=0&&choice!=1)
	{
		cout<<"Do you want to shutdown this application? (1= yes, 0= no)"<<endl;
		cin>>choice;
	}
	}while(choice!=1);
	
	return 0;
}
