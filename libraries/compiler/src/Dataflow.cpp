#include "Dataflow.h"
#include "ScalarVar.h"
#include "Compiler.h"

namespace emll
{
	namespace compiler
	{
		void DataNode::Process(DataFlowGraph& graph, Compiler& compiler)
		{
			Variable* pResult = OnProcess(graph, compiler);
			if (pResult != nullptr)
			{
				NotifyDependencies(graph, compiler, *pResult);
				OnProcessComplete(graph, compiler, *pResult);
			}
		}

		void DataNode::OnProcessComplete(DataFlowGraph& graph, Compiler& compiler, Variable& varResult)
		{
			compiler.FreeVar(varResult);
		}

		void DataNode::NotifyDependencies(DataFlowGraph& graph, Compiler& compiler, Variable& varResult)
		{
			for (size_t i = 0; i < _dependencies.size(); ++i)
			{
				_dependencies[i]->Process(graph, compiler, varResult);
			}
		}

		DataNode* DataFlowGraph::GetNodeAt(size_t offset) const
		{
			return _nodes[offset].get();
		}

		BinaryNode::BinaryNode()
		{
		}

		BinaryNode::~BinaryNode()
		{
		}

		LiteralNode::LiteralNode(Variable* pVar)
			: _pVar(pVar)
		{
			assert(pVar != nullptr);
		}
	}
}