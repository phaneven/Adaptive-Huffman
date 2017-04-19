---------------
README
---------------
included: 
ahencode.cpp
ahdecode.cpp
Makefile
this README

---------------
Command Line Arguments
---------------

Example: ./ahencode -s; ./ahencode; ./ahdecode -s; ./ahdecode

-s
 if the executable is ahencode, it can seperate the output by each byte;
 if the executable is ahdecode, it can receive seperate digital string;

---------------
Algorithm Analysis and some methods' specification 
---------------

(1) building tree process:
 Every time a byte inserted into the adapative huffman tree, it needs to update the tree. The algorithm should guarantee from bottom to up and from left to right the order of nodes is increasing, besides leaf ndetails about update tree:
The logic is every time we want to add an item in the tree. We should first check whether the item have already in the tree and swap one by one to the largest order node with the same weight as the item.
  If the item didn't exited in the tree, we find NYT, make a new NYT(left child) and a new Node(right child) containing the item. Both two node's weight is 0 currently. So, new node do not need any move, we move to its parent and check slide and increment rule, until we reach the root;
  If the item existed in the tree, we find the node, we check all the leafnode with the same weight, and swap them one by one until the item's node get the largest weight. Then we add its weight, and find its current parent and do slide and increment from bottom to top; but there exist some special situation: there is no node to swap with the item, which means the item has the smallest order among all leaf nodes. So we find its parent and do slide and increment, when we have updated all ancestors, we add the item's weight.

(2) define block:
  If the node is a leaf node, its block contains all the internal nodes whose weight equals to the leaf node's weight;
  If the node is internal node, its block contains all the leaf nodes whose weight equals to the interanl node's weight + 1;
(Notice: not including the root node)  

(3) slide and increment:
  If the node's block is empty, we just increasing the node's weight and check its parent node;
  If the node's block is not empty, we swap the node with the nodes in its block until it becomes the lagest order one. Then we add the node's weight. If the node is a leaf node we check its new parent; otherwise we check its intial parent.

(4) encoder method:
  The program read each byte from the input string and check whether it have already exited in the adaptive huffman tree:
  
  <situation1>  If the node did not appeared before and the tree is just contains a root, we add the translation of the byte to digital string and update tree;
  
  <situation2>  If the node did not appeared before and the tree is not just contains a root, we find the path first and translate the byte, then add the path and utf code of the byte into the output digital string and update tree;
  
  <situation3>  Else, the node have already appeared, so we just add its path into the output digital string and update tree.

(5) decoder method: 
  UTF-8 format:
     beginwith 0xxxxxxx -> 1 byte
               110xxxxx -> 2 bytes
               1110xxxx -> 3 bytes
               11110xxx -> 4 bytes
               111110xx -> 5 bytes
               1111110x -> 6 bytes 
[when the tree only has a root]
  The program read first 8 digits as a byte. We check how many bytes we still need to read to finish a completed character. And we create a string c to store the completed character(add valid 8 bits each time, not including digits that represent path):

***complete character process****
  If the first 8 digits indicates no more byte need to read, we add the 8-bit-represented symbol into output string and update the tree.
  If the first 8 digits indicates there are still n bytes to read. we first continuous reading from the input string to find the path until reach the NYT node or a leaf node. (a) if it is NYT, then next 8 bits is new. We read the 8 digits and update the tree. (b) if it is a leaf node, we just read the node content and update the tree.
  Util string c is completed, we add the string c to the output string.
*********************************

[when the tree not only has a root]
  find path first until reach the NYT or Leaf node;
  If the node is NYT, read next 8 bit then do the same thing as complete character process;
  If the node is a leaf node, read content from the node then do the same thing as complete character process.


---------------
Design issues and summary
---------------
In my first version I put a completed character into one node, rather than each node for one byte. Until check with the auto test 3, I found the problem and fixed it. During this process i found store each byte in each node is more space-saved.

