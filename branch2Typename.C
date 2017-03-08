#include "TTree.h"
#include "TBranchElement.h"
#include "TBranchClones.h"
#include "TH1F.h"
#include "TClonesArray.h"

#include <iostream>
#include <vector>
#include <list>
#include <sstream>

using BranchNames = std::vector<std::string>;


#ifdef __ROOTCLING__
#pragma link C++ class vector<double*>+;
#pragma link C++ class list<char>+;
#pragma link C++ class A+;
#endif

class A{};

std::string GetBranchTypeName(TBranch* branch) {
   static const TClassRef tbranchelRef("TBranchElement");
   if (branch->InheritsFrom(tbranchelRef)) {
      return static_cast<TBranchElement*>(branch)->GetClassName();
   } else { // Try the fundamental type
      auto title = branch->GetTitle();
      auto typeCode = title[strlen(title) - 1];
      if (typeCode == 'B') return "char";
      else if (typeCode == 'b') return "unsigned char";
      else if (typeCode == 'I') return "int";
      else if (typeCode == 'i') return "unsigned int";
      else if (typeCode == 'S') return "short";
      else if (typeCode == 's') return "unsigned short";
      else if (typeCode == 'D') return "double";
      else if (typeCode == 'F') return "float";
      else if (typeCode == 'L') return "Long64_t";
      else if (typeCode == 'l') return "ULong64_t";
      else if (typeCode == 'O') return "bool";
   }
   return "";
}

TTree* gt = nullptr;

std::string BuildMethodCallCode(std::string_view thisTypeName, std::string_view methodName, const BranchNames& bn, void* thisPtr)
{
   std::stringstream ss;
   ss << "((" << thisTypeName << "*)" << thisPtr << ")->" << methodName << "<";
   const auto bnSize(bn.size());
   unsigned int i = 0;
   while(i < bnSize) {
      auto& branchName = bn[i];
      auto branch = gt->GetBranch(branchName.c_str());
      ss << GetBranchTypeName(branch);
      if (++i != bnSize) {
         ss << ", ";
      }
   }
   i = 0;
   ss << ">({";
   while(i < bnSize) {
      auto& branchName = bn[i];
      ss << "\"" << branchName << "\"";
      if (++i != bnSize) {
         ss << ", ";
      }
   }
   ss << "});";
   return ss.str();
}

void branch2Typename(){

   TTree t("myTree","myTree");
   gt = &t;
   double _double;
   int _int;
   vector<float> _vector_float;
   list<char> _list_char;
   vector<double*> _vector_doublep;
   TH1F _th1f;
   A _a;
   TClonesArray _clones_tnamed("TNamed");
   t.Branch("_double", &_double);
   t.Branch("_int", &_int);
   t.Branch("_vector_float", &_vector_float);
   t.Branch("_list_char", &_list_char);
   t.Branch("_vector_doublep", &_vector_doublep);
   t.Branch("_th1f", &_th1f);
   t.Branch("_a", &_a);
   t.Branch("_clones_tnamed", &_clones_tnamed);

   for (auto bo : *t.GetListOfBranches()) {
      auto b = (TBranch*) bo;
      std::cout << "Branch name " << b->GetName()
                << "\t\t" << GetBranchTypeName(b) << std::endl;
   }

   auto mcallCode = BuildMethodCallCode("ROOT::Experimental::TDataFrameInterface<ROOT::Detail::TDataFrameFilterBase>",
                                        "Histo1D",
                                        {"_double", "_vector_doublep", "_th1f"},
                                        nullptr);
   std::cout << mcallCode << std::endl;

}
