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
			//n.feed(fV,3);
			n.feed(fV,3);
			n.makeReady();
			//cout<<"chan no of a "<<a.tmpoch()<<"\n";
			//cout<<"chan no of b "<<b.tmpoch()<<"\n";
			//cout<<"chan no of c "<<c.tmpoch()<<"\n";
			auto ttf = d.getO();
			cout<<"dim is "<<ttf.getChannel()<<"\n";
	system("pause");
	return 0;
}
// in the log in console there is a entry thread id 0 deleted
// the below things are not the case now..... 18/8/13-11:35 
// message is mutex destroyed while busy......
// simply implies that mutex is not freed properly
// the debug version is terminating abruptly but the release version is ok with correct result...