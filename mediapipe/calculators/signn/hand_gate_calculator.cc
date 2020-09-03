#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/landmark.pb.h"

#include "mediapipe/calculators/signn/timed_queue.h"

namespace mediapipe{

    namespace{
        constexpr char NormalizedLandmarks[] = "LANDMARKS";
        constexpr char Allow[] = "ALLOW";
        constexpr char Disallow[] = "DISALLOW";
    }

    class HandGateCalculator: public CalculatorBase {
    public:
    HandGateCalculator(){};
    ~HandGateCalculator(){};

    static ::mediapipe::Status GetContract(CalculatorContract* cc){
        cc->Inputs().Tag(NormalizedLandmarks).Set<std::vector<NormalizedLandmarkList>>();
        cc->Outputs().Tag(NormalizedLandmarks).Set<NormalizedLandmarkList>();
        cc->Outputs().Tag(Allow).Set<bool>();
        cc->Outputs().Tag(Disallow).Set<bool>();
        return ::mediapipe::OkStatus();
    }
    ::mediapipe::Status Open(CalculatorContext* cc){
        for(int i = 0; i < 21; i++){
            NormalizedLandmark* scored = last_hand.add_landmark();
            scored->set_x(1);
            scored->set_y(1);
            scored->set_z(1);
        }
        hand_history = TimedQueue<int>(2 * 1000);
        return ::mediapipe::OkStatus();
    }
    ::mediapipe::Status Process(CalculatorContext* cc){
        std::vector<NormalizedLandmarkList> hands = cc->Inputs().Tag(NormalizedLandmarks).Get<std::vector<NormalizedLandmarkList>>();
        NormalizedLandmarkList hand;
        if(hands.size() != 1){
            hand = last_hand;
        }else{
            hand = hands.at(0);
            last_hand = hand;
        }
        hand_history.add(hands.size());
        std::vector<int> v = hand_history.get();
        double average = 1.0 * std::accumulate(v.begin(), v.end(), 0LL) / v.size();
        LOG(INFO) << average;
        std::unique_ptr<NormalizedLandmarkList> output_stream_collection = std::make_unique<NormalizedLandmarkList>(hand); 
        cc -> Outputs().Tag(NormalizedLandmarks).Add(output_stream_collection.release(), cc->InputTimestamp());
        cc->Outputs().Tag(Allow).AddPacket(MakePacket<bool>(Allow).At(cc->InputTimestamp()));
        cc->Outputs().Tag(Disallow).AddPacket(MakePacket<bool>(Disallow).At(cc->InputTimestamp()));
        return ::mediapipe::OkStatus();
    }
    ::mediapipe::Status Close(CalculatorContext* cc){
        return ::mediapipe::OkStatus();
    }

    private:
    NormalizedLandmarkList last_hand;
    TimedQueue<int> hand_history; 
    };
    REGISTER_CALCULATOR(HandGateCalculator);
}