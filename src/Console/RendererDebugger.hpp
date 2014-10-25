#pragma once

#include <vector>
#include <map>

#include <GeoGen/GeoGen.hpp>
#include "CommandTable.hpp"

namespace geogen
{
	namespace console
	{
		class RendererDebugger
		{
			renderer::Renderer renderer;
			CommandTable commandTable;

			IStream& in;
			OStream& out;

			String outputDirectory;

			bool aborted = false;
			bool autosave = false;

			unsigned currentStepNumber = 0;

			double lastStepTime = 0;

			// Non-copyable
			RendererDebugger(RendererDebugger const&) : renderer(*(renderer::RenderingSequence*)NULL), in(Cin), out(Cout) {};
			RendererDebugger& operator=(RendererDebugger const&) {};
		public:
			RendererDebugger(geogen::IStream& in, geogen::OStream& out, renderer::RenderingSequence const& sequence, String outputDirectory);
			~RendererDebugger() {};

			inline IStream& GetIn() { return this->in; }
			inline OStream& GetOut() { return this->out; }

			inline String GetOutputDirectory() { return this->outputDirectory; }

			inline renderer::Renderer* GetRenderer() { return &this->renderer; };
			inline CommandTable& GetCommandTable() { return this->commandTable; };

			inline void Abort() { this->aborted = true; }
			inline bool IsAborted() const { return this->aborted; }

			inline void SetAutosave(bool autosave) { this->autosave = autosave; }
			inline bool GetAutosave() const { return this->autosave; }

			inline double GetLastStepTime() const { return this->lastStepTime; }

			void Run();

			void Step();
		};
	}
}