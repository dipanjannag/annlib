#include "stdafx.h"
#include "CppUnitTest.h"
#include "PerceptronLayer.h"
#include "network.h"
#include <chrono>
#include <ctime>
#include <ratio>
#include <stdlib.h>
using namespace std;
using namespace std::chrono;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace AnnUnitTest
{		
	TEST_CLASS(BasicUnitTest)
	{
	public:
		
		TEST_METHOD(RuntimeTest)
		{
			int unitSize = 100000;
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
			auto resss = c.getout()->at(0);
			auto t2 = Clock::now();
			duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
			auto cont =  time_span.count();
			char vOutChar[26];
			_gcvt_s(vOutChar, sizeof(vOutChar), cont, 17);
			wchar_t vOut[26];
			mbstowcs_s(NULL, vOut, sizeof(vOut) / 2, vOutChar, sizeof(vOutChar));
			Logger l;
			l.WriteMessage("Time taken for Computation: ");
			l.WriteMessage(vOut);
			// TODO: Your test code here
		}

	};
}