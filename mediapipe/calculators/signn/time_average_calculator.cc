#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/landmark.pb.h"
#include "mediapipe/calculators/signn/time_average_calculator.pb.h"

#include <chrono>
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
        memory = options.memory_in_seconds() * 1000;
        return ::mediapipe::OkStatus();
    }
    ::mediapipe::Status Process(CalculatorContext* cc){
        NormalizedLandmarkList hand = cc -> Inputs().Tag(NormalizedLandmarks).Get<NormalizedLandmarkList>();
        std::chrono::_V2::system_clock::time_point now = std::chrono::high_resolution_clock::now();
        times.push(now);
        landmarks.push_back(hand);
        auto front = times.front();
        while(std::chrono::duration_cast<std::chrono::milliseconds>(now-front).count() > memory){
            landmarks.erase(landmarks.begin());
            times.pop();
            front = times.front();
        }
        NormalizedLandmarkList average;
        for(int i = 0; i < hand.landmark_size(); i++){
            NormalizedLandmark* xyz = average.add_landmark();
            float average_x = 0;
            float average_y = 0;
            float average_z = 0;
            for(int j = 0; j < times.size(); j++){
                average_x += landmarks.at(j).landmark(i).x();
                average_y += landmarks.at(j).landmark(i).y();
                average_z += landmarks.at(j).landmark(i).z();
            }
            average_x /= landmarks.size();
            average_y /= landmarks.size();
            average_z /= landmarks.size();
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
    std::queue<std::chrono::_V2::system_clock::time_point> times;
    std::vector<NormalizedLandmarkList> landmarks; 
    float memory = 0;
    
    };
    REGISTER_CALCULATOR(TimeAverageCalculatorOneHand);
}