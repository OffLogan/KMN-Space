#ifndef NOTE_H
#define NOTE_H

#include <string>

class Note
{
private:
    int id_;
    int parentId_;
    std::string title_;
    std::string content_;
    bool isFolder_;

public:
    Note();
    Note(const std::string& title, const std::string& content, int parentId = 0);

    int GetId() const;
    int GetParentId() const;
    const std::string GetTitle() const;
    const std::string GetContent() const;
    bool IsFolder() const;

    bool SetId(int id);
    bool SetParentId(int parentId);
    bool SetTitle(const std::string& title);
    bool SetContent(const std::string& content);
    void SetIsFolder(bool isFolder);
};

#endif
