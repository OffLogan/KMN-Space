//NoteGestor.h
//Header file for the note gestor class

#ifndef NOTEGESTOR_H
#define NOTEGESTOR_H

#include "note.h"

#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

class NoteData
{
private:
    std::vector<Note> notes_;
    std::unordered_map<int, std::size_t> indexById_;
    std::string filePath_;

    void RebuildIndex();

public:
    explicit NoteData(const std::string& filePath = "data/notes.json");
    std::vector<Note> Data() const;
    bool Load();
    bool Update();
    int NextId() const;
    bool AddNote(const Note& note);
    bool RemoveNote(int id);
    bool RemoveNoteCascade(int id);
    bool UpdateNote(const Note& note);
    Note* FindById(int id);
    const Note* FindById(int id) const;
    int ParentIdForCreation(int selectedId) const;
};

#endif
