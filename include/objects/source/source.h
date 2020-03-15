#pragma once

#include "objects/decoder/decoder.h"
#include "objects/sounddata/sounddata.h"
#include "common/stringmap.h"

namespace love
{
    class StaticDataBuffer : public Object
    {
        public:
            StaticDataBuffer(void * data, size_t size);
            ~StaticDataBuffer();

            inline s16 * GetBuffer() const
            {
                return this->buffer.first;
            }

            inline size_t GetSize() const
            {
                return this->buffer.second;
            }

        private:
            std::pair<s16 *, size_t> buffer;
    };

    class Source : public Object
    {
        public:
            static love::Type type;

            enum Type
            {
                TYPE_STATIC,
                TYPE_STREAM,
                TYPE_QUEUE,
                TYPE_MAX_ENUM
            };

            enum Unit
            {
                UNIT_SECONDS,
                UNIT_SAMPLES,
                UNIT_MAX_ENUM
            };

            Source(SoundData * sound);
            Source(Decoder * decoder);
            Source(const Source & other);

            ~Source();

            Source * Clone();

            bool Play();

            void Stop();

            void Pause();

            bool IsPlaying() const;

            bool IsFinished() const;

            void SetVolume(float volume);

            float GetVolume() const;

            Type GetType() const;

            static bool GetConstant(const char * in, Type & out);
            static bool GetConstant(Type in, const char  *& out);
            static std::vector<std::string> GetConstants(Type);

            bool Update();

        protected:
            Type sourceType;

        private:
            void Reset();
            void CreateWaveBuffer(waveBuffer * buffer, size_t size, size_t nsamples);
            void AddWaveBuffer();

            void PrepareAtomic();
            int StreamAtomic(s16 * buffer, Decoder * decoder);
            bool PlayAtomic();
            void ResumeAtomic();

            bool valid = false;

            const static int DEFAULT_BUFFERS = 2;
            const static int MAX_BUFFERS = 2;

            float volume = 1.0f;
            bool looping = false;
            float minVolume = 0.0f;
            float maxVolume = 1.0f;

            int offsetSamples = 0;

            int sampleRate = 0;
            int channels = 0;
            int bitDepth = 0;

            StrongReference<Decoder> decoder;
            StrongReference<StaticDataBuffer> staticBuffer;

            int buffers = 0;

            s16 * audioBuffers[DEFAULT_BUFFERS];
            waveBuffer sources[DEFAULT_BUFFERS];

            bool index = false;

            static StringMap<Type, TYPE_MAX_ENUM>::Entry typeEntries[];
            static StringMap<Type, TYPE_MAX_ENUM> types;
    };
}
