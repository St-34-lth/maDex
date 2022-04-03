/*
  ==============================================================================

    Tokenizer.h
    Created: 4 Feb 2022 8:08:50pm
    Author:  solo

  ==============================================================================
*/

#pragma once

#pragma once
#include <list>
#include <regex>
#include <iostream>
#include <string>
using namespace std;
// A simple tokenizer class that provides static methods to handle strings when used in messages, such as when using an ActionListenerCallback
class Tokenizer
{
          public:
                    Tokenizer();
                    ~Tokenizer();
                    static list<string> tokenizeMessage(string str,
                                                  const regex re)
                    {

                              sregex_token_iterator it{
                              str.begin(),
                              str.end(), re, -1};
                              list<string> tokenized{it, {}};
                              try
                              {

                                        //  removes empty strings
                                        tokenized.erase(
                                        remove_if(tokenized.begin(), tokenized.end(),
                                                  [](string const &s)
                                                  {
                                                            return s.size() == 0;
                                                  }),
                                        tokenized.end());

                                        return tokenized;
                              }
                              catch (exception &e)
                              {
                                        cout << "command tokenizer error : "
                                        << e.what() << '\n';
                              };
                              return tokenized;
                    };
};
