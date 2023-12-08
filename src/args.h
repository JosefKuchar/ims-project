#pragma once

class Args {
   private:
    void validate();

   public:
    Args(int argc, char** argv);
    void printHelp();
};
