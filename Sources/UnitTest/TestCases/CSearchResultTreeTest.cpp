#include <TestCases/CSearchResultTreeTest.h>
#include <boost/foreach.hpp>

#define CREATE_CONNECTION(base_node, new_node, index)\
	SharedTreeNodes new_node(new SearchResultTreeNode(index));\
	EdgesList & edges##new_node = base_node->children[new_node->iCurrentIndex.get()];\
	edges##new_node.second=new_node;


void CSearchResultTreeNodeTest ::init()
{
	root.reset(new SearchResultTreeNode(0));
}
void CSearchResultTreeNodeTest::initialTree()
{
	CREATE_CONNECTION(root, node1_1, 3);
	{
		CREATE_CONNECTION(node1_1, node2_1,4);
		CREATE_CONNECTION(node1_1, node3_1,5);
	}
	CREATE_CONNECTION(root, node1_2, 4);
	{
		CREATE_CONNECTION(node1_2, node2_2,5);
		CREATE_CONNECTION(node1_2, node3_2,6);
	}
	CREATE_CONNECTION(root, node1_3, 5);
	{
		CREATE_CONNECTION(node1_3, node2_3,7);
		CREATE_CONNECTION(node1_3, node3_3,8);
	}
};
void CSearchResultTreeNodeTest ::cleanup()
{
	root.reset();
}
//add word covering first and only first digit
void CSearchResultTreeNodeTest::test_BuildTree_01()
{
	std::string firstWord("First");
	root->addNode(0,0,firstWord);
	QCOMPARE(root->children.size(),(std::size_t)1);
	//test amount of added word on edge (0,0)
	QCOMPARE(root->children[1].first.size(),(std::size_t)1);
	TreeNodesList foundNodes = root->find_node(0);
	QCOMPARE(foundNodes.size(),(std::size_t)1);
	QCOMPARE(foundNodes.front().get(),root.get());
	foundNodes = root->find_node(1);
	QCOMPARE(foundNodes.size(),(std::size_t)1);
	QCOMPARE(root->children[1].second.get(),foundNodes.front().get());
}
void CSearchResultTreeNodeTest::test_BuildTree_02()
{
	std::string firstWord("First");
	std::string secondWord("Second");
	root->addNode(0,0,firstWord);
	root->addNode(1,1,secondWord);
	QCOMPARE(root->children.size(),(std::size_t)1);
	//second node with Startingindex=1 - starting point for second digit
	TreeNodesList foundNodes = root->find_node(1);
	QCOMPARE(foundNodes.size(),(std::size_t)1);
	EdgesList & edges_1 = foundNodes.front()->children[2];
	QCOMPARE(edges_1.first.size(),(std::size_t)1);
	QCOMPARE(edges_1.first.front(),secondWord);
	foundNodes = root->find_node(2);
	QCOMPARE(edges_1.second.get(),foundNodes.front().get());
}
void CSearchResultTreeNodeTest::test_BuildTree_03()
{
	std::string firstWord("First");
	std::string firstWord2("First First");
	std::string firstWord3("First2");

	root->addNode(0,0,firstWord);
	root->addNode(0,0,firstWord3);
	root->addNode(0,2,firstWord2);
	QCOMPARE(root->children.size(),(std::size_t)2);
	EdgesList & edges_0_0 = root->children[1];
	QCOMPARE(edges_0_0.first.size(),(std::size_t)2);
	QCOMPARE(edges_0_0.first.front(),firstWord);
	QCOMPARE(edges_0_0.first.back(),firstWord3);
	EdgesList & edges_0_2 = root->children[3];
	QCOMPARE(edges_0_2.first.size(),(std::size_t)1);
	QCOMPARE(edges_0_2.first.front(),firstWord2);
}
void CSearchResultTreeNodeTest ::test_FindFirstLevel()
{
	initialTree();
	TreeNodesList foundNodes = root->find_node(0);
	QCOMPARE(foundNodes.size(),(std::size_t)1);
	QCOMPARE(foundNodes.front().get(),root.get());
}
void CSearchResultTreeNodeTest ::test_FindSecondLevel()
{
	initialTree();
	TreeNodesList foundNodes = root->find_node(3);
	QCOMPARE(foundNodes.size(),(std::size_t)1);
	QCOMPARE(foundNodes.front()->iCurrentIndex.get(),(unsigned int)3);
}
void CSearchResultTreeNodeTest ::test_FindOnDiffLevel_01()
{
	initialTree();
	TreeNodesList foundNodes = root->find_node(4);
	QCOMPARE(foundNodes.size(),(std::size_t)2);
	QCOMPARE(foundNodes.front()->iCurrentIndex.get(),(unsigned int)4);
	QCOMPARE(foundNodes.back()->iCurrentIndex.get(),(unsigned int)4);
}
void CSearchResultTreeNodeTest ::test_FindOnDiffLevel_02()
{
	initialTree();
	TreeNodesList foundNodes = root->find_node(5);
	QCOMPARE(foundNodes.size(),(std::size_t)3);
	QCOMPARE(foundNodes.front()->iCurrentIndex.get(),(unsigned int)5);
	QCOMPARE(foundNodes.back()->iCurrentIndex.get(),(unsigned int)5);
}
