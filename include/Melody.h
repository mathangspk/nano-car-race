#pragma once
#include <stddef.h> // <- Thêm dòng này để có size_t

struct Note
{
    int frequency;
    int duration;
};

class Melody
{
public:
    Melody(const Note *notes, size_t length, bool loop = false)
        : _notes(notes), _length(length), _loop(loop) {}

    const Note *getNotes() const { return _notes; }
    size_t getLength() const { return _length; }
    bool shouldLoop() const { return _loop; }
    void setLoop(bool loop) { _loop = loop; }

private:
    const Note *_notes;
    size_t _length;
    bool _loop;
};
