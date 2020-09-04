#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/landmark.pb.h"
#include "mediapipe/calculators/signn/time_average_calculator.pb.h"
#include "mediapipe/calculators/signn/timed_queue.h"

#include <vector>
#include <queue>


namespace mediapipe{
    namespace{
        constexpr char NormalizedLandmarks[] = "LANDMARKS";
    }

    class TimeAverageCalculatorOneHand: public CalculatorBase {
    public:
    TimeAverageCalculatorOneHand(){};
    ~TimeAverageCalculatorOneHand(){};

    static ::mediapipe::Status GetContract(CalculatorContract* cc){
        cc->Inputs().Tag(NormalizedLandmarks).Set<NormalizedLandmarkList>();
        cc->Outputs().Tag(NormalizedLandmarks).Set<NormalizedLandmarkList>();
        return ::mediapipe::OkStatus();
    }
    ::mediapipe::Status Open(CalculatorContext* cc){
        const auto& options = cc->Options<::mediapipe::TimeAverageCalculatorOptions>();
        history = TimedQueue<NormalizedLandmarkList>(options.memory_in_seconds());
        return ::mediapipe::OkStatus();
    }
    ::mediapipe::Status Process(CalculatorContext* cc){
        NormalizedLandmarkList hand = cc -> Inputs().Tag(NormalizedLandmarks).Get<NormalizedLandmarkList>();
        history.add(hand);
        auto times = history.get();
        NormalizedLandmarkList average;
        for(int i = 0; i < hand.landmark_size(); i++){
            NormalizedLandmark* xyz = average.add_landmark();
            float average_x = 0;
            float average_y = 0;
            float average_z = 0;
            for(int j = 0; j < times.size(); j++){
                average_x += times.at(j).landmark(i).x();
                average_y += times.at(j).landmark(i).y();
                average_z += times.at(j).landmark(i).z();
            }
            average_x /= times.size();
            average_y /= times.size();
            average_z /= times.size();
            xyz->set_x(average_x);
            xyz->set_y(average_y);
            xyz->set_z(average_z);
        }
        std::unique_ptr<NormalizedLandmarkList> output_stream_collection = std::make_unique<NormalizedLandmarkList>(average);
        cc -> Outputs().Tag(NormalizedLandmarks).Add(output_stream_collection.release(), cc->InputTimestamp());
        return ::mediapipe::OkStatus();
    }
    ::mediapipe::Status Close(CalculatorContext* cc){
        return ::mediapipe::OkStatus();
    }
    private:
    float memory = 0;
    TimedQueue<NormalizedLandmarkList> history;
    
    };
    REGISTER_CALCULATOR(TimeAverageCalculatorOneHand);
}