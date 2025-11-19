
namespace NS {
  struct S {};
  void fn(auto&&);
} // namespace NS

void use(){
  NS::S{}.fn(); // this shouldnt work, missing ADL filtering
}
