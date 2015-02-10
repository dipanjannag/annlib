#include "network.h"
using namespace ann;


namespace ann{
	enum sourceType
	{
		file,
		fileStream,
		buffer,
		xmlFile
	};
	template<typename T> class BasicLearner
	{
	public:
		BasicLearner(network<T> net) : targetNetwork(net)
		{

		}
		void bindTrainer(string filename)
		{

		}
		void bindNetwork(string filename)
		{

		}
		~BasicLearner();

	private:
		const network<T>& targetNetwork;
		string trainerSourceName;
		sourceType trainerSourceType;
		string netOutputName;
		sourceType netOutputType;


		void _train_iteration(FeatureSet<T> trainerInput)
		{
			targetNetwork.feed(trainerInput);
			targetNetwork.makeReady();

			for (size_t i = 0; i < targetNetwork.getEndLayer().getUnitCount() ; i++)
			{
				auto out = targetNetwork.getEndLayer().getout()->at(i);
				for (size_t j = 0; j < targetNetwork.getEndLayer().getInputDimension() ; j++)
				{

				}
			}
		}
	};
}