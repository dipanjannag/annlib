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
			vector<int> fV;
			for(int i(0);i<9;i++)
				fV.push_back(i);


			a.connectTo(b);
			a.connectTo(c);
			b.connectTo(d);
			c.connectTo(d);
			network<int> n(a);
			n.addEndPoint(d);
			n.feed(fV);
			n.makeReady();
			Assert::AreEqual(72,d.tmpchk());
		}
		TEST_METHOD(channelTest1)
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
			n.feed(fV);
			n.makeReady();
			Assert::AreEqual(72,d.tmpchk());
			Assert::AreEqual(1,(int)d.tmpoch());
		}
		TEST_METHOD(channelTest3)
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
			n.feed(fV,3);
			n.makeReady();
			Assert::AreEqual(72,d.tmpchk());
			//Assert::AreEqual(3,(int)a.tmpoch());
			//Assert::AreEqual(3,(int)b.tmpoch());
			//Assert::AreEqual(3,(int)c.tmpoch());
			Assert::AreEqual(3,(int)d.tmpoch());
		}

	};
}
