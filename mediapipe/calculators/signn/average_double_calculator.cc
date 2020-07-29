#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/calculators/signn/average_double_calculator.pb.h"

#include <vector>

namespace mediapipe{

    namespace{
        constexpr char DOUBLE[] = "DOUBLE";
    }

    class AverageDoubleCalculator : public CalculatorBase {
        public:
        AverageDoubleCalculator(){};
        ~AverageDoubleCalculator(){};

        static ::mediapipe::Status GetContract(CalculatorContract* cc){
            cc->Inputs().Tag(DOUBLE).Set<double>();
            cc->Outputs().Tag(DOUBLE).Set<double>();
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Open(CalculatorContext* cc){
            const auto& options = cc->Options<::mediapipe::AverageDoubleCalculatorOptions>();
            double average_frames = options.average_of_x_frames();
            max_history = (int) average_frames;
            for(int i = 0; i < max_history; i++){
                history.push_back(0);
            }
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Process(CalculatorContext* cc){
            history.at(current) = cc -> Inputs().Tag(DOUBLE).Get<double>();
            current++;
            if(current == max_history){
                current = 0;
            }
            double average = 0;
            for(int i = 0; i < max_history; i++){
                average += history.at(i);
            }
            average /= history.size();
            std::unique_ptr<double> output_stream_collection = std::make_unique<double>(average); 
            cc -> Outputs().Tag(DOUBLE).Add(output_stream_collection.release(), cc->InputTimestamp());
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Close(CalculatorContext* cc){
            return ::mediapipe::OkStatus();
        }

        private:
        std::vector<double> history;
        int max_history;
        int current = 0;

        

    };
    
    REGISTER_CALCULATOR(AverageDoubleCalculator);
}