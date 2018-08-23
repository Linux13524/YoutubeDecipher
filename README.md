# Youtube-Decipher
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/a904a29e8a57484cb79c6d39d9da7061)](https://www.codacy.com/app/Linux13524/Youtube-Decipher?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=Linux13524/Youtube-Decipher&amp;utm_campaign=Badge_Grade)

## Description:
This c++-library is able do decipher signatures of Youtube-Videos.

## Usage:
- Add the library into your cmake build
- Include the `decipher.h` into your code
- call `Youtube::Decipher::instance(videoHtml).DecipherSignature(signature)`

   `videoHtml`: 
   - Any Youtube video HTML (used to extract the ytplayer.js)
      
      It is not necessary to provide the whole html due to simple regexing (see section "How does it work?") 
      
   `signature`:
   - The signature you want to decipher
   
## Dependencies:
Uses **Boost-Beast** for network access and **Boost-Regex** for regexing the html and javascript files.

Uses **Open SSL** to perform https request

## How does it work?
There are several steps needed to obtain a list of primitive functions (see: [Subfunction Types](#subfunction-types)) to be called on the ciphered signature to decipher it.

#### LoadDecipherJavaScript:
The decipher function is stored in a JavaScript file.

##### Input: 
Youtube video HTML string  
##### RegEx: 
<pre>(ytplayer.config\s=\s{.*"assets":({.*?}))</pre>  
##### RegEx Match (Group 2): 
<pre>{ "css" : ... , "js" : <b>path-to-javascript</b> }</pre>  
##### Output: 
JavaScript file ( = "s.ytimg.com/**path-to-javascript**")  
  
#### LoadDecipherFunctionName:
The JavaScript file is obfuscated; we need to find the correct decipher functions name

##### Input:
Decipher JavaScript file  
##### RegEx:
<pre>("signature",(\w\w))</pre>  
##### RegEx Match (Group 1):
Two char long function name  
##### Output:
Two char long function name 

#### LoadDecipherFunctionDefinition:
Find the decipher function in the JavaScript file

##### Input:
Decipher JavaScript file + function name  
##### RegEx:
<pre><b>function-name</b>(=function\(.+?\)\{(.+?)\})</pre>  
##### RegEx Match:
Function definition
##### Output:
Function definition

#### LoadSubFunctionName:
The decipher function uses some primitive subfunctions to change the signature.  
All subfunctions are in the same namespace.  

##### Input:
Decipher function definition
##### RegEx:
<pre>((..)\.(..)\(.,(\d)+\))</pre>  
##### RegEx Match:
Every match will be a subfunction call with its arguments: <pre><b>namespace</b>.<b>subfunction</b>(..., <b>argument</b>)</pre>
##### Output:
Namespace name

#### LoadSubFunctionDefinition:
Load namespace defintion containing all subfunction definitions.

##### Input:
Decipher JavaScript file + Namespace name
##### RegEx:
<pre>(var\s)<b>namespace</b>(={((?:\n|.)*?)};)</pre>  
##### RegEx (Group 2):
Namespace definition
##### Output:
Namespace definition

#### ExtractSubFuncNames:
Load the subfunctions names and store them together with the type (see: [Subfunction Types](#subfunction-types)) and the argument 

##### Input:
Namespace definition
##### RegEx:
<pre>((\w\w):function\(.+?\){(.*?)})</pre>  
##### RegEx (Group 1):
Subfunction names
##### RegEx (Group 2):
Subfunction definitions

#### ExtractDecipher:
Load the subfunction names from the decipher function and store them in a list

##### Input:
Decipher function definition
##### RegEx:
<pre>(\.(..)\(.,(\d+)\))</pre>  
##### RegEx:
Every match will contain a subfunction name

### Subfunction Types:
Every Subfunction has an input string (the signature). Some may also have an argument (integer).

#### Reverse:
Reverses the signature  
**js**: `signature.reverse()`  
**c++**: `std::reverse(signature->begin(), signature->end())`  

#### Splice:
Removes a part of the signature (beginning to arguments position)  
**js**: `signature.splice(0, argument)`  
**c++**: `signature->erase(0, argument)`  

#### Swap:
Swaps two chars of the signature (first char with char at arguments position)  
**js**: `var c=signature[0]; signature[0]=signature[argument%signature.length]; signature[argument%signature.length]=c`  
**c++**: `signature->erase(0, argument)`  
