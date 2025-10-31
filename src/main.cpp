#include <cstddef>
#include <algorithm>
#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <cstdlib>
#include <optional>
#include <string>
#include <utility>
#include <fstream>
#include <stack>

enum class EJointTag
{
  EJT_UNDEF, 
  EJT_ROOT, 
  EJT_JOINT, 
  EJT_END, 
};

struct FOffset
{
  float x,y,z;
};

struct FBVHNode
{
  FBVHNode* Parent{};
  std::vector<FBVHNode*> Children;
  EJointTag Type; 
};

class UBiovisionHierarchyFile
{
  std::vector<EJointTag> m_hierarchy;
  const uint32_t m_motion_offset; 

public:
  UBiovisionHierarchyFile(std::ifstream& f);
  uint32_t GetMotionOffset() const; 
};

[[maybe_unused]] std::optional<std::pair<std::string, std::string>> ParseArg( const std::string& str , char delim = '=' )
{
  std::string::const_iterator begin{ str.begin() };
  std::string::const_iterator end{ str.end() };
  if( std::string::const_iterator it {std::find(begin, end, delim)} ; it != end )
  {
    std::ptrdiff_t idx{ it - begin };
    std::pair<std::string, std::string> V{str.substr(0, idx), str.substr(idx + 1, str.size())};
    return V;
  }
  return {};
}


[[maybe_unused]] std::vector<std::string> tokenize_line(const std::string& line)
{
  /* TODO: Make sure trailing whitesapce is trimmed aswell */ 
  
  std::string::const_iterator begin { line.cbegin() };
  std::string::const_iterator end   { line.end()    };
  const std::string::difference_type count { std::count(begin, end, ' ') + 1 } ; 

  std::vector<std::string> Tokenized; Tokenized.reserve(count);

  std::string::size_type slow{}; 
  std::string::size_type fast{}; 
  for( std::string::value_type i : line )
  {
    if( i == ' ' && slow != fast)
    {
      Tokenized.emplace_back(line.substr(slow, fast - slow));
      fast++;
      slow = fast;
    }
    else
      fast++;
  }
  Tokenized.emplace_back(line.substr(slow, fast - slow));
  return Tokenized; 
}


/* Read stripped whitespace assumes no fully tabbed lines? */ 
/* TODO: trim trailing whtiespace otherwise bug will arise with tokenize_line */ 

std::string trim_whitespace(std::string value)
{
  std::string::size_type idx{}; 
  while(value[idx] == '\t') idx++ ; 
  return value.substr(idx, value.size()); 
}


int main (int argc, char *argv[]) 
{
  if(argc <= 1)
  {
    std::cout << "Usage: ./transform --input=file.bvh" << std::endl;
    return EXIT_FAILURE; 
  }

  auto Values{ ParseArg(argv[1]) }; 
  if(!Values.has_value())
    return EXIT_FAILURE; 

  std::ifstream file{Values->second, std::ios_base::in};

  if(!file.is_open())
  {
    std::cout << "Could not open file" << std::endl;
    return EXIT_FAILURE; 
  }
  std::string buffer; 

  /* 
  std::stack<char> Tree; 
  int tabs{}; 
  while(std::getline(file, buffer))
  {
    std::string line = trim_whitespace(buffer);
    char current = line[0];
    if(current == '{')
    {
      std::cout << std::string(tabs,' ') << current << std::endl;
      tabs++; 
      Tree.push('{');
    }
    else if(current == '}')
    {
      tabs--;
      std::cout << std::string(tabs,' ') << current << std::endl;
      Tree.push('}');
    }
    else 
    {
      tokenize_line(line);
    }
  }
  */


  auto V = tokenize_line("Hello World There Ayub");
  for( auto i : V )
    std::cout << i << std::endl;

  return EXIT_SUCCESS;
}
