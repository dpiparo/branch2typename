#include "TTree.h"
#include "TBranchElement.h"
#include "TBranchClones.h"
#include "TH1F.h"
#include "TClonesArray.h"

#include <iostream>
#include <vector>
#include <list>

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

void branch2Typename(){

   TTree t("myTree","myTree");
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

}
