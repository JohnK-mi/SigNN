#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/calculators/signn/timed_queue.h"
#include "mediapipe/framework/formats/landmark.pb.h"

#include <vector>




namespace mediapipe{

    namespace{
        constexpr char SIGNAL[] = "SIGNAL";
        constexpr char DOUBLE[] = "DOUBLE";
        constexpr char Landmarks[] = "LANDMARKS";
    }

    class FPSGateCalculator: public CalculatorBase {
        public:
        FPSGateCalculator(){};
        ~FPSGateCalculator(){};

        static ::mediapipe::Status GetContract(CalculatorContract* cc){
            cc->Inputs().Tag(DOUBLE).Set<double>();
            cc->Inputs().Tag(Landmarks).Set<NormalizedLandmarkList>();
            cc->Outputs().Tag(SIGNAL).Set<bool>();
            cc->Outputs().Tag(Landmarks).Set<NormalizedLandmarkList>();
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Open(CalculatorContext* cc){
            history = TimedQueue<double>(2);
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Process(CalculatorContext* cc){
            history.add(cc->Inputs().Tag(DOUBLE).Get<double>());
            std::unique_ptr<NormalizedLandmarkList> output_stream_collection = std::make_unique<NormalizedLandmarkList>(cc->Inputs().Tag(Landmarks).Get<NormalizedLandmarkList>());
            std::vector<double> fps_over_time = history.get();
            double average = 0;
            for(int i = 0; i < fps_over_time.size(); i++){
                average += fps_over_time[i];
            }
            average /= fps_over_time.size();
            if(average <= 3){
                std::unique_ptr<bool> output_stream_collection = std::make_unique<bool>(true); 
                cc -> Outputs().Tag(SIGNAL).Add(output_stream_collection.release(), cc->InputTimestamp()); 
            }else{
                cc -> Outputs().Tag(Landmarks).Add(output_stream_collection.release(), cc->InputTimestamp());
            }

            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Close(CalculatorContext* cc){
            return ::mediapipe::OkStatus();
        }

        private:
        TimedQueue<double> history;

    };
    REGISTER_CALCULATOR(FPSGateCalculator);
}