
void Function(auto ... args){
  auto Lambda = [args...](){};
}

void Invoke(){
  Function(1, 'a', "bcd");
}
