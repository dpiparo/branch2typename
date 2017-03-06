#include "TTree.h"
#include "TBranchElement.h"
#include "TH1F.h"

#include <iostream>
#include <vector>
#include <list>

#ifdef __ROOTCLING__
#pragma link C++ class vector<double*>+;
#pragma link C++ class list<char>+;
#endif

std::string GetBranchTypeName(TBranch* branch) {
   auto branchEl = dynamic_cast<TBranchElement *>(branch);
   if (!branchEl) { // This is a fundamental type
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
   } else {
      return branchEl->GetTypeName();
   }
   return "CANNOT DETERMINE TYPE!";
}

void branch2Typename(){

   TTree t("myTree","myTree");
   double _double;
   int _int;
   vector<float> _vector_float;
   list<char> _list_char;
   vector<double*> _vector_doublep;
   TH1F _th1f;
   t.Branch("_double", &_double);
   t.Branch("_int", &_int);
   t.Branch("_vector_float", &_vector_float);
   t.Branch("_list_char", &_list_char);
   t.Branch("_vector_doublep", &_vector_doublep);
   t.Branch("_th1f", &_th1f);

   for (auto bo : *t.GetListOfBranches()) {
      auto b = (TBranch*) bo;
      std::cout << "Branch name " << b->GetName() << " type " << GetBranchTypeName(b) << std::endl;
   }

}