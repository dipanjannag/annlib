// fann_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include "fann.h"
#include <conio.h>
#include <chrono>
#include <iostream>

using namespace std;
using namespace std::chrono;

int main(int argc, const char* argv[])
{
	/*try{
		const unsigned int num_input = 1000000;
		const unsigned int num_output = 1;
		const unsigned int num_layers = 3;
		const unsigned int num_neurons_hidden = 3;
		const float desired_error = (const float) 0.001;
		const unsigned int max_epochs = 500000;
		const unsigned int epochs_between_reports = 1000;

		struct fann *ann = fann_create_standard(num_layers, num_input, num_neurons_hidden, num_output);

		fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
		fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);

		fann_train_on_file(ann, "large_train.txt", max_epochs, epochs_between_reports, desired_error);

		fann_save(ann, "large_net.net");

		fann_destroy(ann);
	}
	catch (exception e)
	{
		cout << e.what();
	}
	system("pause");
	return 0;*/
	//return 0;
	int unitSize = 0;
	char* filename;
	if (argc == 2)
	{
		const char* arg = argv[1];
		
		//cout << arg << "\n";
		if (strcmp(arg, "-eel") == 0)
		{
			//unitSize = 100000;
			unitSize = 1000000;
			filename = "large_net.net";
			//cout << "elarge\n";
		}
		else if (strcmp(arg, "-el") == 0)
		{
			unitSize = 1000000;
			filename = "large_net.net";
			//cout << "elarge\n";
		}
		else if (strcmp(arg, "-l") == 0)
		{
			unitSize = 1000000;
			filename = "large_net.net";
			//cout << "large\n";
		}
		else if (strcmp(arg, "-m") == 0)
		{
			unitSize = 120000;
			filename = "med_net.net";
			//cout << "medium\n";
		}
		else if (strcmp("-s", arg) == 0)
		{
			unitSize = 12000;
			filename = "small_net.net";
			//cout << "small\n";
		}
		else if (strcmp("-es", arg) == 0)
		{
			unitSize = 6000;
			filename = "small_net.net";

			//cout << "small\n";
		}
		else
		{
			cout << "invalid option extered. \nEciting. . .\n";
			exit(0);
		}
	}
	typedef std::chrono::high_resolution_clock Clock;
	
	fann_type *calc_out;
	fann_type* input = new fann_type[unitSize];
	for (size_t i = 0; i < unitSize; i++)
	{
		input[i] = 1;
	}
	auto t1 = Clock::now();
	struct fann *ann = fann_create_from_file(filename);
	calc_out = fann_run(ann, input);
	auto ans = calc_out[0];
//	printf("xor test (%f,%f) -> %f\n", input[0], input[1], calc_out[0]);
	auto t2 = Clock::now();
	
	fann_destroy(ann);
	//system("pause");
	
	auto chk = t2 - t1;
	duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
	cout << "\nTime taken for computation: " << time_span.count() << "\n";
	//cout << "\nCalculated answer: " << calc_out[0] << "\n";
	
	return 0;


	
}

