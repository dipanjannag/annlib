#include "stdafx.h"
#include "CppUnitTest.h"
#include "ann\PerceptronLayer.h"
#include "ann\FeatureSet.h"
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
			PerceptronLayer<int> a;
			PerceptronLayer<int> b;
			PerceptronLayer<int> c;
			PerceptronLayer<int> d;
			PerceptronLayer<int> e;
			PerceptronLayer<int> f;
			PerceptronLayer<int> g;
			PerceptronLayer<int> h;
			PerceptronLayer<int> i;
			PerceptronLayer<int> j;
			vector<int> fV;
			for(int i(0);i<5;i++)
				fV.push_back(i);
			a.connectTo(b);
			a.connectTo(e);
			b.connectTo(c);
			b.connectTo(d);
			c.connectTo(f);
			d.connectTo(f);
			e.connectTo(f);
			a.feedv(fV);
			//system("pause");
			int temp = f.getout()->size();
			Logger::WriteMessage("size is ");
			Assert::AreEqual(15,temp);
				// TODO: Your test code here
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
