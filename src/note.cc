#include "../include/note.h"

Note::Note()
    : id_(0), parentId_(0), title_(), content_(), isFolder_(false) {}

Note::Note(const std::string& title, const std::string& content, int parentId)
    : id_(0), parentId_(0), title_(), content_(), isFolder_(false)
{
    SetParentId(parentId);
    SetTitle(title);
    SetContent(content);
}

int Note::GetId() const
{
    return id_;
}

int Note::GetParentId() const
{
    return parentId_;
}

const std::string Note::GetTitle() const
{
    return title_;
}

const std::string Note::GetContent() const
{
    return content_;
}

bool Note::IsFolder() const
{
    return isFolder_;
}

bool Note::SetId(int id)
{
    if (id <= 0) {
        return false;
    }

    id_ = id;
    return true;
}

bool Note::SetParentId(int parentId)
{
    if (parentId < 0) {
        return false;
    }

    parentId_ = parentId;
    return true;
}

bool Note::SetTitle(const std::string& title)
{
    title_ = title;
    return true;
}

bool Note::SetContent(const std::string& content)
{
    content_ = content;
    return true;
}

void Note::SetIsFolder(bool isFolder)
{
    isFolder_ = isFolder;
}
