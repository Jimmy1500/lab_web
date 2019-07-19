main: src/main.cpp
	$(CXX) -std=c++17 -I/usr/local/include/Poco -o $@ $< -L/usr/local/lib -lPocoFoundation -lPocoData -lPocoDataMySQL
