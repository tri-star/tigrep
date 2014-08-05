#include <iostream>
#include <boost/regex.hpp>

using namespace std;

int main(int argc, char** argv) {
  //string input = "<tag>test-string</tag>";
  string input = "<tag>";
  
  boost::smatch matches;
  try {
    boost::regex expression("<[a-zA-Z]+>(.*?)</[a-zA-Z]+>");
    
    if(!boost::regex_search(input, matches, expression)) {
      throw new runtime_error("regex not matched.");
    }
    
    cout << "matched-result: " << matches.str(1) << endl;
    
  } catch(runtime_error* e) {
    cout << e->what() << "\n";
    delete e;
  }
  
  return 0;
}

