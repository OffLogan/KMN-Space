//ReminderGestor.cc
//Implementation file for the reminder data gestion class

#include "../include/reminderGestor.h"

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

ReminderData::ReminderData(const std::string& filePath)
    : reminders_(), indexById_(), filePath_(filePath) {}

std::vector<Reminder> ReminderData::Data() const {return reminders_;}

void ReminderData::RebuildIndex()
{
    indexById_.clear();

    for (std::size_t i = 0; i < reminders_.size(); ++i) {
        indexById_[reminders_[i].GetTaskId()] = i;
    }
}

bool ReminderData::Load()
{
    QFile file(resolvedDataPath(filePath_));

    if (!file.exists()) {
        reminders_.clear();
        indexById_.clear();
        return Update();
    }

    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    const QByteArray rawData = file.readAll();
    file.close();

    if (rawData.trimmed().isEmpty()) {
        reminders_.clear();
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
    std::vector<Reminder> loadedReminders;
    loadedReminders.reserve(static_cast<std::size_t>(items.size()));

    for (const QJsonValue& value : items) {
        if (!value.isObject()) {
            return false;
        }

        const QJsonObject reminderObject = value.toObject();
        if (!reminderObject.contains("id") || !reminderObject.contains("name") ||
            !reminderObject.contains("description") || !reminderObject.contains("due")) {
            return false;
        }

        Reminder reminder;
        if (!reminder.SetId(reminderObject.value("id").toInt())) {
            return false;
        }
        if (!reminder.SetName(reminderObject.value("name").toString().toStdString())) {
            return false;
        }
        if (!reminder.SetDescription(reminderObject.value("description").toString().toStdString())) {
            return false;
        }
        if (!reminder.SetDue(reminderObject.value("due").toString().toStdString())) {
            return false;
        }
        reminder.SetStatus(reminderObject.value("status").toBool(false));

        loadedReminders.push_back(reminder);
    }

    reminders_ = std::move(loadedReminders);
    RebuildIndex();
    return true;
}

bool ReminderData::Update()
{
    QJsonArray items;

    for (const Reminder& reminder : reminders_) {
        QJsonObject reminderObject;
        reminderObject.insert("id", reminder.GetTaskId());
        reminderObject.insert("name", QString::fromStdString(reminder.GetName()));
        reminderObject.insert("description", QString::fromStdString(reminder.GetDescription()));
        reminderObject.insert("status", reminder.GetStatus());
        reminderObject.insert("due", QString::fromStdString(reminder.GetDue()));
        items.append(reminderObject);
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

int ReminderData::NextId() const
{
    int maxId = 0;

    for (const Reminder& reminder : reminders_) {
        if (reminder.GetTaskId() > maxId) {
            maxId = reminder.GetTaskId();
        }
    }

    return maxId + 1;
}

bool ReminderData::AddReminder(const Reminder& reminder)
{
    if (reminder.GetTaskId() <= 0 || indexById_.count(reminder.GetTaskId()) > 0) {
        return false;
    }

    reminders_.push_back(reminder);
    indexById_[reminder.GetTaskId()] = reminders_.size() - 1;
    return Update();
}

bool ReminderData::RemoveReminder(const int id)
{
    const auto foundReminder = indexById_.find(id);
    if (foundReminder == indexById_.end()) {
        return false;
    }

    reminders_.erase(reminders_.begin() + static_cast<std::ptrdiff_t>(foundReminder->second));
    RebuildIndex();
    return Update();
}
