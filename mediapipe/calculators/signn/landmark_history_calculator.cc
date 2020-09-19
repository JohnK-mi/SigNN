#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/landmark.pb.h"

#include "mediapipe/calculators/signn/timed_queue.h"
#include "mediapipe/calculators/signn/landmark_history_calculator.pb.h"

#include <vector>


namespace mediapipe{

    namespace{
        constexpr char NormalizedLandmarks[] = "LANDMARKS";
        constexpr char LandmarksHistory[] = "LANDMARKS_HISTORY";
    }

    class LandmarkHistoryCalculator : public CalculatorBase {
        public:
        LandmarkHistoryCalculator(){};
        ~LandmarkHistoryCalculator(){};

        static ::mediapipe::Status GetContract(CalculatorContract* cc){
            cc->Inputs().Tag(NormalizedLandmarks).Set<NormalizedLandmarkList>();
            cc->Outputs().Tag(LandmarksHistory).Set<std::vector<NormalizedLandmarkList>>();
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Open(CalculatorContext* cc){
            const auto& options = cc->Options<::mediapipe::LandmarkHistoryCalculatorOptions>();
            hand_history = TimedQueue<NormalizedLandmarkList>(options.memory_in_seconds());
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Process(CalculatorContext* cc){
            NormalizedLandmarkList hand = cc->Inputs().Tag(NormalizedLandmarks).Get<NormalizedLandmarkList>();
            hand_history.add(hand);
            auto history = hand_history.get();

            std::unique_ptr<std::vector<NormalizedLandmarkList>> output_stream_collection = std::make_unique<std::vector<NormalizedLandmarkList>>(hand_history.get()); 
            cc -> Outputs().Tag(LandmarksHistory).Add(output_stream_collection.release(), cc->InputTimestamp());
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Close(CalculatorContext* cc){
            return ::mediapipe::OkStatus();
        }
        private:
        TimedQueue<NormalizedLandmarkList> hand_history; 


    };
    REGISTER_CALCULATOR(LandmarkHistoryCalculator);
}