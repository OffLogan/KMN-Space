//TaskGestor.cc
//Implementation file for the task data gestion class

#include "../include/taskGestor.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

namespace {
QString resolvedDataPath(const std::string& filePath)
{
    const QString configuredPath = QString::fromStdString(filePath);
    const QFileInfo directInfo(configuredPath);
    if (directInfo.exists() || directInfo.isAbsolute()) {
        return directInfo.filePath();
    }

    const QString applicationPath = QCoreApplication::applicationDirPath();
    const QStringList candidatePaths = {
        QDir::cleanPath(QDir::current().absoluteFilePath(configuredPath)),
        QDir::cleanPath(applicationPath + "/../" + configuredPath),
        QDir::cleanPath(applicationPath + "/../../" + configuredPath),
        QDir::cleanPath(applicationPath + "/../../../" + configuredPath)
    };

    for (const QString& candidate : candidatePaths) {
        if (QFileInfo::exists(candidate)) {
            return candidate;
        }
    }

    return candidatePaths.back();
}
}

TaskData::TaskData(const std::string& filePath)
    :   tasks_(), indexById_(), filePath_(filePath) {}

std::vector<Task> TaskData::Data() const {return tasks_;}

void TaskData::RebuildIndex()
{
    indexById_.clear();

    for (std::size_t i = 0; i < tasks_.size(); ++i) {
        indexById_[tasks_[i].GetTaskId()] = i;
    }
}

bool TaskData::Load()
{
    QFile file(resolvedDataPath(filePath_));

    if (!file.exists()) {
        tasks_.clear();
        indexById_.clear();
        return Update();
    }

    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    const QByteArray rawData = file.readAll();
    file.close();

    if (rawData.trimmed().isEmpty()) {
        tasks_.clear();
        indexById_.clear();
        return Update();
    }

    const QJsonDocument document = QJsonDocument::fromJson(rawData);
    if (!document.isObject()) {
        return false;
    }

    const QJsonObject rootObject = document.object();
    const QJsonValue itemsValue = rootObject.value("items");
    if (!itemsValue.isArray()) {
        return false;
    }

    const QJsonArray items = itemsValue.toArray();
    std::vector<Task> loadedTasks;
    loadedTasks.reserve(static_cast<std::size_t>(items.size()));

    for (const QJsonValue& value : items) {
        if (!value.isObject()) {
            return false;
        }

        const QJsonObject taskObject = value.toObject();
        if (!taskObject.contains("id") || !taskObject.contains("name") || !taskObject.contains("description")) {
            return false;
        }

        Task task;
        if (!task.SetId(taskObject.value("id").toInt())) {
            return false;
        }
        if (!task.SetName(taskObject.value("name").toString().toStdString())) {
            return false;
        }
        if (!task.SetDescription(taskObject.value("description").toString().toStdString())) {
            return false;
        }
        task.SetStatus(taskObject.value("status").toBool(false));

        loadedTasks.push_back(task);
    }

    tasks_ = std::move(loadedTasks);
    RebuildIndex();
    return true;
}

bool TaskData::Update()
{
    QJsonArray items;

    for (const Task& task : tasks_) {
        QJsonObject taskObject;
        taskObject.insert("id", task.GetTaskId());
        taskObject.insert("name", QString::fromStdString(task.GetName()));
        taskObject.insert("description", QString::fromStdString(task.GetDescription()));
        taskObject.insert("status", task.GetStatus());
        items.append(taskObject);
    }

    QJsonObject rootObject;
    rootObject.insert("version", 1);
    rootObject.insert("items", items);

    const QString outputPath = resolvedDataPath(filePath_);
    const QFileInfo fileInfo(outputPath);
    QDir().mkpath(fileInfo.path());

    QFile file(outputPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return false;
    }

    const QJsonDocument document(rootObject);
    const qint64 writtenBytes = file.write(document.toJson(QJsonDocument::Indented));
    file.close();

    return writtenBytes != -1;
}

int TaskData::NextId() const
{
    int maxId = 0;

    for (const Task& task : tasks_) {
        if (task.GetTaskId() > maxId) {
            maxId = task.GetTaskId();
        }
    }

    return maxId + 1;
}

bool TaskData::AddTask(const Task& task)
{
    if (task.GetTaskId() <= 0 || indexById_.count(task.GetTaskId()) > 0) {
        return false;
    }

    tasks_.push_back(task);
    indexById_[task.GetTaskId()] = tasks_.size() - 1;
    return Update();
}

bool TaskData::RemoveTask(const int id)
{
    const auto foundTask = indexById_.find(id);
    if (foundTask == indexById_.end()) {
        return false;
    }

    tasks_.erase(tasks_.begin() + static_cast<std::ptrdiff_t>(foundTask->second));
    RebuildIndex();
    return Update();
}
