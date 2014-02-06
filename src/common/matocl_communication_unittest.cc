#include "common/matocl_communication.h"

#include <gtest/gtest.h>

#include "unittests/chunk_type_constants.h"
#include "unittests/inout_pair.h"
#include "unittests/operators.h"
#include "unittests/packet.h"

TEST(MatoclCommunicationTests, FuseReadChunkData) {
	LIZARDFS_DEFINE_INOUT_PAIR(uint32_t, messageId,    512, 0);
	LIZARDFS_DEFINE_INOUT_PAIR(uint64_t, chunkId,      87,  0);
	LIZARDFS_DEFINE_INOUT_PAIR(uint32_t, chunkVersion, 52,  0);
	LIZARDFS_DEFINE_INOUT_PAIR(uint64_t, fileLength,   124, 0);
	LIZARDFS_DEFINE_INOUT_VECTOR_PAIR(ChunkTypeWithAddress, serverList) = {
		ChunkTypeWithAddress(NetworkAddress(0xC0A80001, 8080), standard),
		ChunkTypeWithAddress(NetworkAddress(0xC0A80002, 8081), xor_p_of_6),
		ChunkTypeWithAddress(NetworkAddress(0xC0A80003, 8082), xor_1_of_6),
		ChunkTypeWithAddress(NetworkAddress(0xC0A80004, 8084), xor_5_of_7),
	};

	std::vector<uint8_t> buffer;
	ASSERT_NO_THROW(matocl::fuseReadChunk::serialize(buffer,
			messageIdIn, fileLengthIn, chunkIdIn, chunkVersionIn, serverListIn));

	verifyHeader(buffer, LIZ_MATOCL_FUSE_READ_CHUNK);
	removeHeaderInPlace(buffer);
	verifyVersion(buffer, matocl::fuseReadChunk::kResponsePacketVersion);
	ASSERT_NO_THROW(deserializePacketDataNoHeader(buffer, messageIdOut));
	ASSERT_NO_THROW(matocl::fuseReadChunk::deserialize(buffer,
			fileLengthOut, chunkIdOut, chunkVersionOut, serverListOut));

	LIZARDFS_VERIFY_INOUT_PAIR(messageId);
	LIZARDFS_VERIFY_INOUT_PAIR(chunkId);
	LIZARDFS_VERIFY_INOUT_PAIR(chunkVersion);
	LIZARDFS_VERIFY_INOUT_PAIR(fileLength);
	LIZARDFS_VERIFY_INOUT_PAIR(serverList);
}

TEST(MatoclCommunicationTests, FuseReadChunkStatus) {
	LIZARDFS_DEFINE_INOUT_PAIR(uint32_t, messageId, 512, 0);
	LIZARDFS_DEFINE_INOUT_PAIR(uint8_t,  status,    10,  0);

	std::vector<uint8_t> buffer;
	ASSERT_NO_THROW(matocl::fuseReadChunk::serialize(buffer, messageIdIn, statusIn));

	verifyHeader(buffer, LIZ_MATOCL_FUSE_READ_CHUNK);
	removeHeaderInPlace(buffer);
	verifyVersion(buffer, matocl::fuseReadChunk::kStatusPacketVersion);
	ASSERT_NO_THROW(deserializePacketDataNoHeader(buffer, messageIdOut));
	ASSERT_NO_THROW(matocl::fuseReadChunk::deserialize(buffer, statusOut));

	LIZARDFS_VERIFY_INOUT_PAIR(messageId);
	LIZARDFS_VERIFY_INOUT_PAIR(status);
}

TEST(MatoclCommunicationTests, FuseWriteChunkData) {
	LIZARDFS_DEFINE_INOUT_PAIR(uint32_t, messageId,    512, 0);
	LIZARDFS_DEFINE_INOUT_PAIR(uint64_t, chunkId,      87,  0);
	LIZARDFS_DEFINE_INOUT_PAIR(uint32_t, chunkVersion, 52,  0);
	LIZARDFS_DEFINE_INOUT_PAIR(uint64_t, fileLength,   124, 0);
	LIZARDFS_DEFINE_INOUT_PAIR(uint32_t, lockId,       225, 0);
	LIZARDFS_DEFINE_INOUT_VECTOR_PAIR(ChunkTypeWithAddress, serverList) = {
			ChunkTypeWithAddress(NetworkAddress(0xC0A80001, 8080), standard),
			ChunkTypeWithAddress(NetworkAddress(0xC0A80002, 8081), xor_p_of_6),
			ChunkTypeWithAddress(NetworkAddress(0xC0A80003, 8082), xor_1_of_6),
			ChunkTypeWithAddress(NetworkAddress(0xC0A80004, 8084), xor_5_of_7),
	};

	std::vector<uint8_t> buffer;
	ASSERT_NO_THROW(matocl::fuseWriteChunk::serialize(buffer,
			messageIdIn, fileLengthIn, chunkIdIn, chunkVersionIn, lockIdIn, serverListIn));

	verifyHeader(buffer, LIZ_MATOCL_FUSE_WRITE_CHUNK);
	removeHeaderInPlace(buffer);
	verifyVersion(buffer, matocl::fuseWriteChunk::kResponsePacketVersion);
	ASSERT_NO_THROW(deserializePacketDataNoHeader(buffer, messageIdOut));
	ASSERT_NO_THROW(matocl::fuseWriteChunk::deserialize(buffer,
			fileLengthOut, chunkIdOut, chunkVersionOut, lockIdOut, serverListOut));

	LIZARDFS_VERIFY_INOUT_PAIR(messageId);
	LIZARDFS_VERIFY_INOUT_PAIR(chunkId);
	LIZARDFS_VERIFY_INOUT_PAIR(chunkVersion);
	LIZARDFS_VERIFY_INOUT_PAIR(fileLength);
	LIZARDFS_VERIFY_INOUT_PAIR(serverList);
}

TEST(MatoclCommunicationTests, FuseWriteChunkStatus) {
	LIZARDFS_DEFINE_INOUT_PAIR(uint32_t, messageId, 512, 0);
	LIZARDFS_DEFINE_INOUT_PAIR(uint8_t,  status,    10,  0);

	std::vector<uint8_t> buffer;
	ASSERT_NO_THROW(matocl::fuseWriteChunk::serialize(buffer, messageIdIn, statusIn));

	verifyHeader(buffer, LIZ_MATOCL_FUSE_WRITE_CHUNK);
	removeHeaderInPlace(buffer);
	verifyVersion(buffer, matocl::fuseReadChunk::kStatusPacketVersion);
	ASSERT_NO_THROW(deserializePacketDataNoHeader(buffer, messageIdOut));
	ASSERT_NO_THROW(matocl::fuseWriteChunk::deserialize(buffer, statusOut));

	LIZARDFS_VERIFY_INOUT_PAIR(messageId);
	LIZARDFS_VERIFY_INOUT_PAIR(status);
}

TEST(MatoclCommunicationTests, FuseWriteChunkEnd) {
	LIZARDFS_DEFINE_INOUT_PAIR(uint32_t, messageId, 512, 0);
	LIZARDFS_DEFINE_INOUT_PAIR(uint8_t,  status,    10,  0);

	std::vector<uint8_t> buffer;
	ASSERT_NO_THROW(matocl::fuseWriteChunkEnd::serialize(buffer, messageIdIn, statusIn));

	verifyHeader(buffer, LIZ_MATOCL_FUSE_WRITE_CHUNK_END);
	removeHeaderInPlace(buffer);
	ASSERT_NO_THROW(deserializePacketDataNoHeader(buffer, messageIdOut));
	ASSERT_NO_THROW(matocl::fuseWriteChunkEnd::deserialize(buffer, statusOut));

	LIZARDFS_VERIFY_INOUT_PAIR(messageId);
	LIZARDFS_VERIFY_INOUT_PAIR(status);
}