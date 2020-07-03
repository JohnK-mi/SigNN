#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/landmark.pb.h"
#include "mediapipe/framework/formats/detection.pb.h"
#include "mediapipe/framework/port/ret_check.h"


#include <vector>
#include <string>
#include <iostream>


namespace mediapipe{

    namespace{
        constexpr char NormalizedLandmarks[] = "LANDMARKS";
    }


    class ZScoreCalculator : public CalculatorBase {
        public:
        ZScoreCalculator(){};
        ~ZScoreCalculator(){};

        static ::mediapipe::Status GetContract(CalculatorContract* cc){
            cc->Inputs().Tag(NormalizedLandmarks).Set<std::vector<NormalizedLandmarkList>>();
            cc->Outputs().Tag(NormalizedLandmarks).Set<std::vector<NormalizedLandmarkList>>();
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Open(CalculatorContext* cc){
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Process(CalculatorContext* cc){
            std::vector<NormalizedLandmarkList> hands = cc -> Inputs().Tag(NormalizedLandmarks).Get<std::vector<NormalizedLandmarkList>>();
            switch(hands.size()){
                case 0:
                    LOG(WARNING) << "No hands detected!\n";
                    cc->Outputs().Tag(NormalizedLandmarks).AddPacket(cc->Inputs().Tag(NormalizedLandmarks).Value());
                    return ::mediapipe::OkStatus();
                case 1:
                    break;
                default:
                    LOG(WARNING) << hands.size() << " hands detected!\n";
                    cc->Outputs().Tag(NormalizedLandmarks).AddPacket(cc->Inputs().Tag(NormalizedLandmarks).Value());
                    return ::mediapipe::OkStatus();
            }
            NormalizedLandmarkList hand = hands.at(0);
            float x_mean = 0;
            float x_sdev = 0;
            float y_mean = 0;
            float y_sdev = 0;
            float z_mean = 0;
            float z_sdev = 0;

            // Find mean
            for(unsigned int i = 0; i < hand.landmark_size(); i++){
                x_mean += hand.landmark(i).x();
                y_mean += hand.landmark(i).y();
                z_mean += hand.landmark(i).z();
            }
            x_mean /= hand.landmark_size();
            y_mean /= hand.landmark_size();

            // Find sdev
            // Σ(xi -mu)^2
            for(unsigned int i = 0; i < hand.landmark_size(); i++){
                x_sdev += powf(hand.landmark(i).x() - x_mean, 2.0);
                y_sdev += powf(hand.landmark(i).y() - y_mean, 2.0);
                z_sdev += powf(hand.landmark(i).z() - z_mean, 2.0);
            }
            // (Σ(xi -mu)^2) / N
            x_sdev /= hand.landmark_size();
            y_sdev /= hand.landmark_size();
            z_sdev /= hand.landmark_size();

            // sqrt((Σ(xi -mu)^2) / N)
            x_sdev = sqrtf(x_sdev);
            y_sdev = sqrtf(y_sdev);
            z_sdev = sqrtf(z_sdev);
            // LOG(INFO) << "X MEAN: " << x_mean << ", X SDEV: " << x_sdev;
            // LOG(INFO) << "YMEAN: " << y_mean << ", Y SDEV: " << y_sdev;


            // get z scores
            NormalizedLandmarkList zscores;
            for(unsigned int i = 0; i < hand.landmark_size(); i++){
                NormalizedLandmark* scored = zscores.add_landmark();
                scored -> set_x((hand.landmark(i).x() - x_mean) / x_sdev);
                scored -> set_y((hand.landmark(i).y() - y_mean) / y_sdev);
                scored -> set_z((hand.landmark(i).z() - z_mean) / z_sdev);
            }

            // send to output
            std::vector<NormalizedLandmarkList> send;
            send.push_back(zscores);
            std::unique_ptr<std::vector<NormalizedLandmarkList>> output_stream_collection = std::make_unique<std::vector<NormalizedLandmarkList>>(send);
            cc -> Outputs().Tag(NormalizedLandmarks).Add(output_stream_collection.release(), cc->InputTimestamp());

            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Close(CalculatorContext* cc){
            return ::mediapipe::OkStatus();
        }

        private:

    };
    REGISTER_CALCULATOR(ZScoreCalculator);
}