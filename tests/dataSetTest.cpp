#include "stdafx.h"
#include "CppUnitTest.h"
#include "Perceptron.h"
#include "PerceptronLayer.h"
#include "FeatureSet.h"
#include "network.h"
#include "unit.h"
#include <chrono>
#include <vector>
#include <string>
using namespace std;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace tests
{		
	TEST_CLASS(coreTest)
	{
	public:
		
		/*TEST_METHOD(outPutCheck)
		{

			unit<float> funit(3);
			unit<float> f(2);
			PerceptronLayer<float> a(funit,3);
			PerceptronLayer<float> b(funit,1);
			PerceptronLayer<float> c(funit,1);
			PerceptronLayer<float> d(f,1);
			vector<float> fV;
			for(int i(0);i<9;i++)
				fV.push_back(10000-i);


			a.connectTo(b);
			a.connectTo(c);
			b.connectTo(d);
			c.connectTo(d);
			network<float> n(a);
			n.addEndPoint(d);
			typedef std::chrono::high_resolution_clock Clock;
			auto t1 = Clock::now();
			
			n.feed(fV);
			n.makeReady();
			auto res = d.tmpchk();
			Assert::AreEqual(0.0f,d.tmpchk());
			auto t2 = Clock::now();
			auto diff = t2 - t1;
			auto at = diff.count();
			
			//Logger::WriteMessage("Test Completed in: "+ to_string(at) );
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
		*/

	};
}
