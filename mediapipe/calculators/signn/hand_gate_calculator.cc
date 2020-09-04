#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/landmark.pb.h"

#include "mediapipe/calculators/signn/timed_queue.h"
#include "mediapipe/calculators/signn/hand_gate_calculator.pb.h"

namespace mediapipe{

    namespace{
        constexpr char NormalizedLandmarks[] = "LANDMARKS";
        constexpr char SIGNAL[] = "SIGNAL";
    }

    class HandGateCalculator: public CalculatorBase {
    public:
    HandGateCalculator(){};
    ~HandGateCalculator(){};

    static ::mediapipe::Status GetContract(CalculatorContract* cc){
        cc->Inputs().Tag(NormalizedLandmarks).Set<std::vector<NormalizedLandmarkList>>();
        cc->Outputs().Tag(NormalizedLandmarks).Set<NormalizedLandmarkList>();
        cc->Outputs().Tag(SIGNAL).Set<bool>();
        return ::mediapipe::OkStatus();
    }
    ::mediapipe::Status Open(CalculatorContext* cc){
        for(int i = 0; i < 21; i++){
            NormalizedLandmark* scored = last_hand.add_landmark();
            scored->set_x(1);
            scored->set_y(1);
            scored->set_z(1);
        }
        const auto& options = cc->Options<::mediapipe::HandGateCalculatorOptions>();
        float memory_length = options.memory_in_seconds();
        accuracy_required = (double) options.percent_of_one_hand_required();

        hand_history = TimedQueue<bool>(1000 * memory_length);
        return ::mediapipe::OkStatus();
    }
    ::mediapipe::Status Process(CalculatorContext* cc){
        std::vector<NormalizedLandmarkList> hands = cc->Inputs().Tag(NormalizedLandmarks).Get<std::vector<NormalizedLandmarkList>>();
        NormalizedLandmarkList hand;
        if(hands.size() != 1){
            hand = last_hand;
            hand_history.add(false);
        }else{
            hand = hands.at(0);
            last_hand = hand;
            hand_history.add(true);
        }
        
        std::vector<bool> v = hand_history.get();
        int trues = 0;
        for(int i = 0; i < v.size(); i++){
            if(v[i] == true){
                trues += 1;
            }
        }
        double average;
        if(v.size() == 0){
            average = 0;
        }else{
            average = trues / v.size();
        }

        // double average = 1.0 * std::accumulate(v.begin(), v.end(), 0LL) / v.size();
        if(accuracy_required <= average){
            std::unique_ptr<NormalizedLandmarkList> output_stream_collection = std::make_unique<NormalizedLandmarkList>(hand); 
            cc -> Outputs().Tag(NormalizedLandmarks).Add(output_stream_collection.release(), cc->InputTimestamp());
            return ::mediapipe::OkStatus();
        }else{
            std::unique_ptr<bool> output_stream_collection = std::make_unique<bool>(true); 
            cc -> Outputs().Tag(SIGNAL).Add(output_stream_collection.release(), cc->InputTimestamp());   
            return ::mediapipe::OkStatus(); 
        }

        // cc->Outputs().Tag(Allow).AddPacket(MakePacket<bool>(Allow).At(cc->InputTimestamp()));
        return ::mediapipe::OkStatus();
    }
    ::mediapipe::Status Close(CalculatorContext* cc){
        return ::mediapipe::OkStatus();
    }

    private:
    NormalizedLandmarkList last_hand;
    TimedQueue<bool> hand_history; 
    double accuracy_required;
    
    };
    REGISTER_CALCULATOR(HandGateCalculator);
}