// ann.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Perceptron.h"
#include "network.h"
#include "FeatureSet.h"
#include "PerceptronLayer.h"
#include "utility.h"
#include "classifier.h"
#include <iostream>
#include <functional>
#include <future>
#include <thread>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <ctime>
#include <ratio>
#include <chrono>

using namespace std::chrono;
using namespace std;
int main(int argc,const char* argv[])
{
	int unitSize = 0;
	if (argc == 2)
	{
		const char* arg = argv[1];
		//cout << arg << "\n";
		if (strcmp(arg, "-eel") == 0)
		{
			//unitSize = 100000;
			unitSize = 4000000;
			//cout << "elarge\n";
		}
		else if (strcmp(arg,"-el" )==0)
		{
			unitSize = 2000000;
			//cout << "elarge\n";
		}
		else if (strcmp(arg,"-l")==0)
		{
			unitSize = 200000;
			//cout << "large\n";
		}
		else if (strcmp(arg,"-m")==0)
		{
			unitSize = 20000;
			//cout << "medium\n";
		}
		else if (strcmp("-s", arg)==0)
		{
			unitSize = 2000;
			//cout << "small\n";
		}
		else if (strcmp("-es", arg)==0)
		{
			unitSize = 1000;
			//cout << "small\n";
		}
		else
		{
			cout << "invalid option extered. \nEciting. . .\n";
			exit(0);
		}
	}
	unit<float> funit(6);
	unit<float> f(2);
	typedef std::chrono::high_resolution_clock Clock;
	auto t1 = Clock::now();
	PerceptronLayer<float> a(funit, unitSize);
	
	PerceptronLayer<float> b(funit, unitSize);
	
	PerceptronLayer<float> c(funit, unitSize);
	/*
	PerceptronLayer<float> d(f, 1);
	*/
	a.connectTo(b);
	b.connectTo(c);
	vector<float> fV;
	for (int i(0); i < (unitSize * 6); i++)
		fV.push_back(0);
	network<float> n(a, c);
	/*
	a.connectTo(c);
	b.connectTo(d);
	c.connectTo(d);
	
	*/
	//n.name = "bitch";
	//n.addEndPoint(d);
	
	//a._feed(fV);
	n.feed(fV);
	n.makeReady();
	auto resss =  c.getout()->at(0);
	auto t2 = Clock::now();
	duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
	cout << "Time taken for Computation: " << time_span.count() <<"\n";
	//cout << "Calculated answer: " << resss << "\n";
	/*vector<float>* cont = new vector<float>();
	a.subscribe(cont);
	while (cont->size()==0)
	{
		this_thread::yield();
	}
	vector<float>* ress = cont;
	*/
 	//auto res = a.tmpchk();
	
	
	//cout << res << "\n";
	
	//res = res;
	//unit<int> funit(3);
	//		unit<int> f(2);
	//		unit<int> fut(1);
	//		PerceptronLayer<int> a(funit,3);
	//		PerceptronLayer<int> b(funit,1);
	//		PerceptronLayer<int> c(funit,1);
	//		PerceptronLayer<int> d(fut,2);
	//		PerceptronLayer<int> e(f,1);
	//		PerceptronLayer<int> fl(f,1);
	//		PerceptronLayer<int> g(f,1);
	//		vector<int> fV;
	//		for(int i(0);i<9;i++)
	//			fV.push_back(i);
	//		a.connectTo(b);
	//		a.connectTo(c);
	//		b.connectTo(d);
	//		c.connectTo(d);
	//		d.connectTo(e);
	//		d.connectTo(fl);
	//		fl.connectTo(g);
	//		e.connectTo(g);
	//		network<int> n(a);
	//		n.addEndPoint(g);
	//		n.feed(fV);
	//		//n.feed(fV,3);
	//		//n.feed(fV,3);
	//		n.init();
	//		n.makeReady();
	//		//cout<<"chan no of a "<<a.tmpoch()<<"\n";
	//		//cout<<"chan no of b "<<b.tmpoch()<<"\n";
	//		//cout<<"chan no of c "<<c.tmpoch()<<"\n";
	//		//auto ttf = d.getO();
	//		//cout<<"dim is "<<ttf.getChannel()<<"\n";
	//		
	//		/*
	//		FeatureSet<float> fts(fV);
	//		normalize(fts);
	//		for(int i(0);i<fts.size();i++)
	//			cout<<fts[i]<<"\t";
	//		linearClassifier<> cl(2,2);
	//		vector<float> fvv(2,0);
	//		FeatureSet<> t1(fvv);
	//		fvv.assign(2,1);
	//		FeatureSet<> t2(fvv);
	//		trainingEntry a1 = make_pair(t1,0);
	//		trainingEntry b1 = make_pair(t2,1);
	//		vector<trainingEntry> ftrn;
	//		ftrn.push_back(a1);
	//		ftrn.push_back(b1);
	//		cl.trnFeed(ftrn);
	//		*/
	////system("pause");
	//_CrtDumpMemoryLeaks();
	//system("pause");
	return 0;
}
// in the log in console there is a entry thread id 0 deleted
// the below things are not the case now..... 18/8/13-11:35 
// message is mutex destroyed while busy......
// simply implies that mutex is not freed properly
// the debug version is terminating abruptly but the release version is ok with correct result...