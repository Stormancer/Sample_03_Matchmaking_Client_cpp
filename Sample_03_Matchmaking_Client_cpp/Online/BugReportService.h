#pragma once
#include "headers.h"
#include <stormancer.h>


namespace Stormancer
{
	struct AttachedFileDescriptor
	{
	public:
		std::string name;
		std::string contentType;
		void* content;
		int contentLength;
	};

	struct BugReportAttachedFile
	{
	public:
		std::string name;
		std::string mimeType;
		int length;
	public:
		MSGPACK_DEFINE(name, mimeType, length);
	};

	struct BugReportMetadata
	{
	public:
		std::string category;
		std::string comment;
		std::vector<BugReportAttachedFile> files;
	public:
		MSGPACK_DEFINE(category, comment, files);
	};

	

	class BugReportService
	{
	public:
		BugReportService(Stormancer::Scene* scene);

		//Reports a bug with optional attached files
		pplx::task<std::shared_ptr<Result<void>>> ReportBug(std::string category, std::string comments, std::vector<AttachedFileDescriptor> files);

	private:
		Stormancer::Scene* _scene;
		std::shared_ptr<Stormancer::IRpcService> _rpc;
	};
}