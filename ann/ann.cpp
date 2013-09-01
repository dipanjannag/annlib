// ann.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Perceptron.h"
#include "network.h"
#include "FeatureSet.h"
#include "PerceptronLayer.h"
#include <iostream>
#include <functional>
#include <future>
#include <thread>

using namespace std;
int _tmain(int argc, _TCHAR* argv[])
{
	//cout<<"suck my dick\n";
	/*
	vector<float> a;
	vector<float> p;
	for(int i(10);i<15;i++)
		p.push_back(i);
	for(int i(0);i<2;i++)
		a.push_back(i);
		*/
	/*
	Feature<float> b([&a]{return a; });
	
	while(b.hasNext())
		cout<< b.next()<<" ";
	cout<<"\n";
	
	b.setFeature([&a]{ 
		vector<float> t;
		t.reserve(a.size()+1);
		for(int i(0);i<a.size();i++)
		{
			t.push_back( a.at(i)/2 );
		}
		return t;});
		
	Feature<float> pqr(p);
	FeatureSet<float> f;
	f.addFeature(b);
	f.putFeature(pqr,1);
	while(f.hasNext())
		std::cout<< f.next()<<" ";
	std::cout<<"\n";
	
	for(int i(0);i<b.getDim();i++)
	{
		std::cout<<f[i]<<" ";
	}
	*/
	/*
	PerceptronLayer<float> x;
	Perceptron<float> pq(3);
	Perceptron<float> pqr(3);
	x.addPerceptron(pq);	// capacity is being reset in this line....
	x.addPerceptron(pqr);
	Feature<float> fet(2,1);
	Feature<float> fet1(2,8);
	FeatureSet<float> fs;
	fs.addFeature(fet);
	fs.addFeature(fet1);
	x.feed(fs);		//here capacity is reset to zero.... How and why????
	auto t= x.getOutput();
	while(t.hasNext())
	{
		cout<<t.next()<<" ";
	}
	
	//vector<int> abc(a.begin(),a.begin()+3);
	
	// the problem is with compiler supplied copy constructor.....
	// either i've to give write access or i've to specialise copy constructor
	*/
	/*
	Perceptron<float> pq(3);
	pq.feed(a);
	Perceptron<float> qp(pq);
	*/
	unit<int> funit(3);
			unit<int> f(2);
			PerceptronLayer<int> a(funit,3);
			PerceptronLayer<int> b(funit,1);
			PerceptronLayer<int> c(funit,1);
			PerceptronLayer<int> d(f,1);
			vector<int> fV;
			for(int i(0);i<9;i++)
				fV.push_back(i);


			a.connectTo(b);
			a.connectTo(c);
			b.connectTo(d);
			c.connectTo(d);
			network<int> n(a);
			n.addEndPoint(d);
			n.feed(fV,3);
			n.makeReady();
			//cout<<"chan no of a "<<a.tmpoch()<<"\n";
			//cout<<"chan no of b "<<b.tmpoch()<<"\n";
			//cout<<"chan no of c "<<c.tmpoch()<<"\n";
			cout<<"chan no of d "<<d.tmpoch()<<"\n";
	system("pause");
	
	/*
	if(n.ready())
	{
		cout<<"ready\n";
	}
	else
	{
		cout<<"false\n";
	}
	*/
	//f.getout()->print();
	//cout<<"\n";
	//f.getout()->print();
	//system("pause");
	return 0;
}
// in the log in console there is a entry thread id 0 deleted
// the below things are not the case now..... 18/8/13-11:35 
// message is mutex destroyed while busy......
// simply implies that mutex is not freed properly
// the debug version is terminating abruptly but the release version is ok with correct result...