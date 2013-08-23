#include "stdafx.h"
#include "CppUnitTest.h"
#include "ann\PerceptronLayer.h"
#include "ann\FeatureSet.h"
#include "ann\network.h"
#include "ann\unit.h"
#include <vector>
using namespace std;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace tests
{		
	TEST_CLASS(coreTest)
	{
	public:
		
		TEST_METHOD(outPutCheck)
		{
			unit<int> funit(3);
			unit<int> f(2);
			PerceptronLayer<int> a(funit,3);
			PerceptronLayer<int> b(funit,1);
			PerceptronLayer<int> c(funit,1);
			PerceptronLayer<int> d(f,1);
	/*
	PerceptronLayer<int> e;
	PerceptronLayer<int> f;
	PerceptronLayer<int> g;
	PerceptronLayer<int> h;
	PerceptronLayer<int> i;
	PerceptronLayer<int> j;
	*/
		vector<int> fV;
		for(int i(0);i<9;i++)
			fV.push_back(i);
	/*
	PerceptronLayer<int> ca;
	PerceptronLayer<int> ac;
	ab.connectTo(ba);
	ab.connectTo(ca);
	ba.connectTo(ac);
	ca.connectTo(ac);
	*/
	//this_thread::sleep_for(chrono::milliseconds(5000));
	
	//ba.connectTo(inter);
	//inter.connectTo(fin);
	//ba.connectTo(fin);
	//aba.connectTo(fin);
	/*
	a.connectTo(b);
	a.connectTo(e);
	b.connectTo(c);
	b.connectTo(d);
	c.connectTo(f);
	d.connectTo(f);
	e.connectTo(f);
	*/
	//f.connectTo(g);
	//f.connectTo(h);
	//g.connectTo(i);
	//h.connectTo(i);
	//i.connectTo(j);
		a.connectTo(b);
		a.connectTo(c);
		b.connectTo(d);
		c.connectTo(d);
		network<int> n(a);
		n.addEndPoint(d);
		a.feedv(fV);
	//d.getout()->print();
	//system("pause");
		n.ready();
				// TODO: Your test code here
		Assert::AreEqual(72,d.tmpchk());
		}

	};
	TEST_CLASS(fSetTest)
	{
	public:
		TEST_METHOD(fetSetTest)
		{
			vector<int> val;
			for (int i = 0; i < 5; i++)
			{
				val.push_back(i);
			}
			FeatureSet<int> a(val);
			auto itr = a.begin();
			for(int i(0);i<a.size();i++)
			{
				Assert::AreEqual(i,a[i]);
				itr++;
			}
			int rfs=0;
			for(auto p(a.begin());p!=a.end();p++)
			{
				Assert::AreEqual(rfs,*p);
				rfs++;
			}
			rfs = 4;
			for(auto p(a.end()-1);p!=a.begin();p--)
			{
				Assert::AreEqual(rfs,*p);
				rfs--;
			}
			/*
			for(int i=a.size();i>=1;i--)
			{
				Assert::AreEqual(i,*itr);
				itr--;
			}
			*/

		}
	};
}
