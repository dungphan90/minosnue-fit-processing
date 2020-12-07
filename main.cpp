#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <utility>
#include <fstream>

#include <TH2.h>
#include <TH1.h>
#include <TGraph.h>
#include <TGraph2D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TFile.h>
#include <TString.h>
#include <TMath.h>

#define max_chi2 50000
#define analysis_chain "ss_combine"
// #define analysis_chain "ss"

std::vector<std::string> ParsingToTokens(std::string s, std::string delimiter) {
    size_t pos = 0;
    std::vector<std::string> tokens;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        std::string token = s.substr(0, pos);
        tokens.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    tokens.push_back(s);

    return tokens;
}

std::vector<double> Parsing(std::string file_name) {
    std::vector<std::string> main_tokens = ParsingToTokens(file_name.c_str(), "_");
    std::vector<std::string> ddcp_tokens = ParsingToTokens(main_tokens.at(4).c_str(), "-");
    std::vector<std::string> th34_tokens = ParsingToTokens(main_tokens.at(5).c_str(), "-");
    std::vector<std::string> th13_tokens = ParsingToTokens(main_tokens.at(6).c_str(), "-");
    std::vector<std::string> th23_tokens = ParsingToTokens(main_tokens.at(7).c_str(), "-");
    std::vector<std::string> dm32_tokens = ParsingToTokens(main_tokens.at(8).c_str(), "-");

    std::vector<double> params;
    params.push_back(std::stod(ddcp_tokens.at(1)));
    params.push_back(std::stod(th34_tokens.at(th34_tokens.size() - 1)));
    params.push_back(std::stod(th13_tokens.at(th13_tokens.size() - 1)));
    params.push_back(std::stod(th23_tokens.at(th23_tokens.size() - 1)));
    params.push_back(std::stod(dm32_tokens.at(dm32_tokens.size() - 1)));

    return params;
}

std::vector<std::tuple<double, double, double>> GetTGraph2DFromFile(std::string& file_name) {
    TFile* root_file = new TFile(file_name.c_str(), "READ");
    TGraph2D* surface = (TGraph2D*) root_file->Get(analysis_chain);

    std::vector<std::tuple<double, double, double>> chi2_combine_surface;
    chi2_combine_surface.clear();

    for (Int_t graph2d_idx = 0; graph2d_idx < surface->GetN(); ++graph2d_idx) {
        double x_current = -1.;
        double y_current = -1.;
        double z_current = -1.;
        surface->GetPoint(graph2d_idx, x_current, y_current, z_current);
        chi2_combine_surface.push_back(std::make_tuple(x_current, y_current, z_current));
    }

    root_file->Close();

    return chi2_combine_surface;
}

std::vector<std::string> ReadFileList(std::string list_file_path, std::string data_file_path) {
    std::vector<std::string> file_list_vector; file_list_vector.clear();
    std::string buffer_str;
    std::ifstream list_file(list_file_path.c_str());
    while(list_file >> buffer_str) {
        file_list_vector.push_back(data_file_path + buffer_str);
    }

    return file_list_vector;
}

std::vector<std::tuple<double, double, double, double, double, double, double, double>> MakeProfiledChi2Surface() {
    TGraph2D* profiled_chi2_graph2d = new TGraph2D();

    double min_log_lower = TMath::Log10(1.0E-3);
    double max_log_lower = TMath::Log10(0.25);
    double step_log_lower = (max_log_lower - min_log_lower) / 51.;
    double min_log_upper = TMath::Log10(0.25);
    double max_log_upper = TMath::Log10(0.5);
    double step_log_upper = (max_log_upper - min_log_upper) / 51.;

    for (unsigned int i = 0; i < 51; ++i) {
        double y_log = min_log_lower + step_log_lower * (double) i;
        double y = TMath::Power(10, y_log);
        for (unsigned int j = 0; j < 51; ++j) {
            double x_log = min_log_lower + step_log_lower * (double) j;
            double x = TMath::Power(10, x_log);
            profiled_chi2_graph2d->SetPoint(profiled_chi2_graph2d->GetN(), x, y, max_chi2);
        }
    }

    for (unsigned int i = 0; i < 51; ++i) {
        double y_log = min_log_lower + step_log_lower * (double) i;
        double y = TMath::Power(10, y_log);
        for (unsigned int j = 50; j < 101; ++j) {
            double x_log = min_log_upper + step_log_upper * (double) (j - 50);
            double x = TMath::Power(10, x_log);
            profiled_chi2_graph2d->SetPoint(profiled_chi2_graph2d->GetN(), x, y, max_chi2);
        }
    }

    for (unsigned int i = 50; i < 101; ++i) {
        double y_log = min_log_upper + step_log_upper * (double) (i - 50);
        double y = TMath::Power(10, y_log);
        for (unsigned int j = 0; j < 51; ++j) {
            double x_log = min_log_lower + step_log_lower * (double) j;
            double x = TMath::Power(10, x_log);
            profiled_chi2_graph2d->SetPoint(profiled_chi2_graph2d->GetN(), x, y, max_chi2);
        }
    }

    for (unsigned int i = 50; i < 101; ++i) {
        double y_log = min_log_upper + step_log_upper * (double) (i - 50);
        double y = TMath::Power(10, y_log);
        for (unsigned int j = 50; j < 101; ++j) {
            double x_log = min_log_upper + step_log_upper * (double) (j - 50);
            double x = TMath::Power(10, x_log);
            profiled_chi2_graph2d->SetPoint(profiled_chi2_graph2d->GetN(), x, y, max_chi2);
        }
    }

    std::vector<std::tuple<double, double, double, double, double, double, double, double>> profiled_chi2_combine_surface;
    for (Int_t graph2d_idx = 0; graph2d_idx < profiled_chi2_graph2d->GetN(); ++graph2d_idx) {
        double x_current = -1.;
        double y_current = -1.;
        double z_current = -1.;
        profiled_chi2_graph2d->GetPoint(graph2d_idx, x_current, y_current, z_current);
        profiled_chi2_combine_surface.push_back(std::make_tuple(x_current, y_current, z_current, -9999., -9999., -9999., -9999., -9999.));
    }

    for (unsigned int i = 0; i < profiled_chi2_combine_surface.size() - 1; ++i) {
        double x_i = std::get<0>(profiled_chi2_combine_surface.at(i));
        double y_i = std::get<1>(profiled_chi2_combine_surface.at(i));
        for (unsigned int j = i + 1; j < profiled_chi2_combine_surface.size(); ++j) {
            double x_j = std::get<0>(profiled_chi2_combine_surface.at(j));
            double y_j = std::get<1>(profiled_chi2_combine_surface.at(j));
            if ((TMath::Abs(x_i - x_j) < 1.0E-5) && (TMath::Abs(y_i - y_j) < 1.0E-5)) {
                profiled_chi2_combine_surface.erase(profiled_chi2_combine_surface.begin() + j);
            }
        }
    }

    std::cout << profiled_chi2_combine_surface.size() << std::endl;

    return profiled_chi2_combine_surface;
}

int main() {
    std::vector<std::string> file_list = ReadFileList("../ListFile.txt", "../../DM0d010/");

    std::vector<std::tuple<double, double, double, double, double, double, double, double>> profiled_tgraph2d = MakeProfiledChi2Surface();

    for (auto& file : file_list) {
        std::vector<std::tuple<double, double, double>> current_tgraph2d = GetTGraph2DFromFile(file);
        std::vector<double> params = Parsing(file.c_str());
        for (auto & ipoint : current_tgraph2d) {
            double x = std::get<0>(ipoint);
            double y = std::get<1>(ipoint);
            double z = std::get<2>(ipoint);
            for (auto & jpoint : profiled_tgraph2d) {
                double profiled_x = std::get<0>(jpoint);
                double profiled_y = std::get<1>(jpoint);
                double profiled_z = std::get<2>(jpoint);
                if ((TMath::Abs(profiled_x - x) < 1.0E-5) && (TMath::Abs(profiled_y - y) < 1.0E-5)) {
                    if (z < profiled_z) {
                        jpoint = std::make_tuple(profiled_x, profiled_y, z, params.at(0)
                                                                          , params.at(1)
                                                                          , params.at(2)
                                                                          , params.at(3)
                                                                          , params.at(4));
                    }
                    break;
                }
            }
        }
    }

    TFile* output_file = new TFile("ProfiledCombineSurface.root", "RECREATE");

    TGraph2D* profiled_chi2_surface = new TGraph2D();
    for (auto & ipoint : profiled_tgraph2d) {
        double x = std::get<0>(ipoint);
        double y = std::get<1>(ipoint);
        double z = std::get<2>(ipoint);
        if (z < max_chi2) {
            profiled_chi2_surface->SetPoint(profiled_chi2_surface->GetN(), x, y, z);
        }
    }
    profiled_chi2_surface->SetName("Chi2Surface");

    TGraph2D* deltacp_bf_surface = new TGraph2D();
    for (auto & ipoint : profiled_tgraph2d) {
        double x = std::get<0>(ipoint);
        double y = std::get<1>(ipoint);
        double z = std::get<3>(ipoint);
        deltacp_bf_surface->SetPoint(deltacp_bf_surface->GetN(), x, y, z);
    }
    deltacp_bf_surface->SetName("DeltaCP_atBF_Surface");

    TGraph2D* theta34_bf_surface = new TGraph2D();
    for (auto & ipoint : profiled_tgraph2d) {
        double x = std::get<0>(ipoint);
        double y = std::get<1>(ipoint);
        double z = std::get<4>(ipoint);
        theta34_bf_surface->SetPoint(theta34_bf_surface->GetN(), x, y, z);
    }
    theta34_bf_surface->SetName("Theta34_atBF_Surface");

    TGraph2D* theta13_bf_surface = new TGraph2D();
    for (auto & ipoint : profiled_tgraph2d) {
        double x = std::get<0>(ipoint);
        double y = std::get<1>(ipoint);
        double z = std::get<5>(ipoint);
        theta13_bf_surface->SetPoint(theta13_bf_surface->GetN(), x, y, z);
    }
    theta13_bf_surface->SetName("Theta13_atBF_Surface");

    TGraph2D* theta23_bf_surface = new TGraph2D();
    for (auto & ipoint : profiled_tgraph2d) {
        double x = std::get<0>(ipoint);
        double y = std::get<1>(ipoint);
        double z = std::get<6>(ipoint);
        theta23_bf_surface->SetPoint(theta23_bf_surface->GetN(), x, y, z);
    }
    theta23_bf_surface->SetName("Theta23_atBF_Surface");

    TGraph2D* deltaM32_bf_surface = new TGraph2D();
    for (auto & ipoint : profiled_tgraph2d) {
        double x = std::get<0>(ipoint);
        double y = std::get<1>(ipoint);
        double z = std::get<7>(ipoint);
        deltaM32_bf_surface->SetPoint(deltaM32_bf_surface->GetN(), x, y, z);
    }
    deltaM32_bf_surface->SetName("DeltaM32_atBF_Surface");

    output_file->cd();
    profiled_chi2_surface->Write();
    deltacp_bf_surface->Write();
    theta34_bf_surface->Write();
    theta13_bf_surface->Write();
    theta23_bf_surface->Write();
    deltaM32_bf_surface->Write();
    output_file->Close();
    
    return 0;
}
