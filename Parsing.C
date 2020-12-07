void Parsing() {
    std::string s = "R2DSSE50S491_3FNovember2020_RP11RP12RP13DATA_Dm41-0.01_DDD-4.7124-0-0_50x50-TypeD-T34-1.5708_th13-0.153_th23-1.119_dmsq32-0.00256.root";
    std::string delimiter = "_";

    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        std::cout << token << std::endl;
        s.erase(0, pos + delimiter.length());
    }
    std::cout << s << std::endl;
}