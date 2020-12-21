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