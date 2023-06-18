#include "game.h"
#include "MinLog.h"

namespace dutycore
{
	char* game::ModuleBase = (char*)GetModuleHandle(NULL);
	game::tGetXAssetSize game::DB_GetXAssetTypeSize = NULL;
	XAsset* game::DB_XAssetPool = NULL;

	XAsset* game::GetXAssetPool(int index)
	{
		main::Log << "DB_XAssetPool " << index  << "\n";
		if (!game::DB_XAssetPool)
		{
			game::DB_XAssetPool = reinterpret_cast<XAsset*>(PTR_GET_XASSET_POOL);
		}
		return &game::DB_XAssetPool[index];
	}

	void game::ResizeAssetLimits(int index, int newSize)
	{
		main::Log << "Resizing asset " << index << " to size of " << newSize;
		main::Log.WriteLine(" ");
		auto structSize = GetXAssetSize(index);
		main::Log << "Getting asset pool" << "\n";
		auto assetPool = GetXAssetPool(index);
		// TODO: Support resizing assetPools to be less than current size
		if (structSize != -1 && assetPool != 0 && assetPool->maximum < newSize)
		{
			auto newBlock = (LinkedListEntry*)calloc(newSize - assetPool->maximum, structSize);
			if (newBlock != 0)
			{
				LinkedListEntry* nextBlockPtr = newBlock;
				if (!assetPool->firstEntry)
				{
					assetPool->firstEntry = newBlock;
					nextBlockPtr = (LinkedListEntry*)((uint8_t*)nextBlockPtr + structSize);
				}

				LinkedListEntry* blockPtr = (LinkedListEntry*)assetPool->firstEntry;

				auto size_c = newSize - 1;
				do
				{
					if (!blockPtr->next)
					{
						blockPtr->next = nextBlockPtr;
						blockPtr = nextBlockPtr;
						nextBlockPtr = (LinkedListEntry*)((uint8_t*)nextBlockPtr + structSize);
					}
					else
					{
						blockPtr = (LinkedListEntry*)((uint8_t*)blockPtr + structSize);
					}
					--size_c;
				} while (size_c);
				assetPool->maximum = newSize;
			}
		}
	}
}