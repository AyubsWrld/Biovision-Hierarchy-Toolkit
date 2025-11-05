#include <cstddef>
#include <algorithm>
#include <iostream>
#include <type_traits>
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
  EJT_OFFSET, 
  EJT_CHANNELS, 
  EJT_JOINT, 
  EJT_END, 
  EJT_MAX, 
};

struct FVector 
{
  float x,y,z  = {} ;

  FVector() = default; 
  FVector(const std::vector<float> values);
};

/* 
struct FOffset : FVector
{
  using Super = FVector ; 
  using Super::Super;

  std::cout << x << std::endl; 

};
*/

static constexpr std::array<std::pair<std::string, EJointTag>, static_cast<std::underlying_type_t<EJointTag>>(EJointTag::EJT_MAX)> IdentifierMap = 
{
  std::pair<std::string,EJointTag>{ "UNDEF" , EJointTag::EJT_UNDEF },
  std::pair<std::string,EJointTag>{ "ROOT" , EJointTag::EJT_ROOT},
  std::pair<std::string,EJointTag>{ "OFFSET" , EJointTag::EJT_OFFSET},
  std::pair<std::string,EJointTag>{ "CHANNELS" , EJointTag::EJT_CHANNELS},
  std::pair<std::string,EJointTag>{ "JOINT" , EJointTag::EJT_JOINT},
  std::pair<std::string,EJointTag>{ "END" , EJointTag::EJT_END},
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

[[__nodiscard__]] std::optional<std::pair<std::string, std::string>> ParseArg( const std::string& str , char delim = '=' )
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

[[__nodiscard__]] constexpr EJointTag _string_to_joint_identifier( const std::string& string_repr )
{
  using enum EJointTag;
  using identifier_map_t = decltype(IdentifierMap) ;

  identifier_map_t::const_iterator    itbegin   { IdentifierMap.cbegin() };
  identifier_map_t::const_iterator    itend     { IdentifierMap.cend()   };

  if( auto it = std::find_if(itbegin, itend, [&](const identifier_map_t::value_type& pair ){ return pair.first == string_repr; }) ; it != itend )
    return it->second;

  return {} ; 
}


[[__nodiscard__]] std::vector<std::string> tokenize_line(const std::string& line)
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

  std::stack<char> Tree; 
  int tabs{}; 
  /*while(std::getline(file, buffer))*/
  /*{*/
  /*  std::string line = trim_whitespace(buffer);*/
  /*  char current = line[0];*/
  /*  if(current == '{')*/
  /*  {*/
  /*    std::cout << std::string(tabs,' ') << current << std::endl;*/
  /*    tabs++; */
  /*    Tree.push('{');*/
  /*  }*/
  /*  else if(current == '}')*/
  /*  {*/
  /*    tabs--;*/
  /*    std::cout << std::string(tabs,' ') << current << std::endl;*/
  /*    Tree.push('}');*/
  /*  }*/
  /*  else */
  /*  {*/
  /*    auto v = tokenize_line(line);*/
  /*  }*/
  /*}*/

  while(std::getline(file, buffer))
  {
    auto identifier = _string_to_joint_identifier(tokenize_line(buffer)[0]);
    switch (identifier) 
    {
      case EJointTag::EJT_UNDEF:
        break;
      case EJointTag::EJT_ROOT:
        break;
      case EJointTag::EJT_OFFSET:
        break;
      case EJointTag::EJT_CHANNELS:
        break;
      case EJointTag::EJT_JOINT:
        break;
      case EJointTag::EJT_END:
        break;
    }
  }
  return EXIT_SUCCESS;
}
