//
// Created by 58226 on 2023/2/6.
//

#ifndef EXTENSIONMETHODS_H
#define EXTENSIONMETHODS_H
#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QTextStream>
#include <QDir>
#include <functional>
#include <type_traits>
namespace ExtensionMethods
{


///
/// The SourcesExtension class , implemented a bunch of methods to enhance operations with sets
///
template<typename finderType>
class SourcesExtension
{
public:
    ///get the first element that matched with condition,need pass the total count of array,if there has no matched one,return defaultValue
    static inline finderType firstOf(const finderType *sources,
                                     const int count,
                                     const std::function<bool(finderType)>& MatchFunction,
                                     const finderType defaultValue)
    {
        finderType result = defaultValue;
        for (int i = 0; i < count; i++) {
            if (MatchFunction(sources[i])) {
                result = sources[i];
                break;
            }
        }
        return result;
    }
    ///get the first element of QList that matched with condition,if there has no matched one,return defaultValue
    static inline finderType firstOf(const QList<finderType> &sources,
                                     const std::function<bool(finderType)>& MatchFunction,
                                     const finderType defaultValue)
    {
        finderType result = defaultValue;
        auto begin = sources.begin();
        while (begin != sources.end()) {
            if (MatchFunction(*begin)) {
                result = *begin;
                break;
            }
            ++begin;
        }
        return result;
    }
    ///get the last element that matched with condition,need pass the total count of array,if there has no marched one,return defaultValue
    static inline finderType lastOf(const finderType *sources,
                                    const int count,
                                    const std::function<bool(finderType)>& MatchFunction,
                                    const finderType defaultValue)
    {
        finderType result = defaultValue;
        for (int i = 0; i < count; i++) {
            if (MatchFunction(sources[i])) {
                result = sources[i];
            }
        }
        return result;
    }
    ///get the last element of QList that matched with condition,if there has no marched one,return defaultValue
    static inline finderType lastOf(const QList<finderType> &sources,
                                    const std::function<bool(finderType)>& MatchFunction,
                                    const finderType defaultValue)
    {
        finderType result = defaultValue;
        auto begin = sources.begin();
        while (begin != sources.end()) {
            if (MatchFunction(*begin)) {
                result = *begin;
            }
            ++begin;
        }
        return result;
    }
    ///get elements which is matched the condition,need pass the total count of array,if there has no matched,return an empty sources
    static inline QList<finderType>
    where(const finderType *sources, const int count,const std::function<bool(finderType)>& MatchFunction)
    {
        QList<finderType> findResult;
        for (int i = 0; i < count; i++) {
            if (MatchFunction(sources[i])) {
                findResult.push_back(sources[i]);
            }
        }
        return findResult;
    }
    ///get elements of QList which is marched the condition,if there has no marched,return an empty sources
    static inline QList<finderType>
    where(const QList<finderType> &sources,const std::function<bool(finderType)>& MatchFunction)
    {
        QList<finderType> findResult;
        auto begin = sources.begin();
        while (begin != sources.end()) {
            if (MatchFunction(*begin)) {
                findResult.push_back(*begin);
            }
            ++begin;
        }
        return findResult;
    }
    ///traverse element in arrays and execute the same function
    static inline void eachBy(const QList<finderType> &sources,const std::function<void(finderType)>& executeFunc)
    {
        auto begin = sources.begin();
        while (begin != sources.end()) {
            executeFunc(*begin);
            ++begin;
        }
    }
    ///traverse element in arrays and execute the same function
    static inline void eachBy(const finderType *sources, int totalCount,const std::function<void(finderType)>& executeFunc)
    {
        for (int i = 0; i < totalCount; i++) {
            executeFunc(sources[i]);
        }
    }
    ///in arrays,skip the {skipCount} number from head,return others
    static inline QList<finderType> skip(const finderType *sources, int totalCount, int skipCount)
    {
        QList<finderType> result;
        finderType *index = sources;
        int leftCount = totalCount - skipCount;
        if (leftCount <= 0)
            return result;
        for (int i = 0; i < skipCount; i++)
            ++index;
        for (int i = 0; i < leftCount; i++) {
            result.push_back(index[i]);
        }
        return result;
    }
    ///in QList,skip the {skipCount} number from head,return others
    static inline QList<finderType> skip(const QList<finderType> &sources, int totalCount, int skipCount)
    {
        QList<finderType> result = QList<finderType>();
        auto index = sources.begin();
        int leftCount = totalCount - skipCount;
        if (leftCount <= 0)
            return result;
        for (int i = 0; i < skipCount; i++)
            ++index;
        while (index != sources.end()) {
            result.push_back(*(++index));
        }
        return result;
    }
    ///in arrays,take the {TakeCount} number from head,if the value was -1,take all of them
    static inline QList<finderType> take(const finderType *sources, int totalCount, int TakeCount = -1)
    {
        QList<finderType> result;
        int limitedCount = TakeCount == -1 ? totalCount : TakeCount;
        for (int curTakeCount = 0; curTakeCount < limitedCount; curTakeCount++) {
            result.push_back(sources[curTakeCount]);
        }
        return result;
    }
    ///in QList,take the {TakeCount} number from head,if the value was -1,take all of them
    static inline QList<finderType> take(const QList<finderType> &sources, int TakeCount = -1)
    {
        QList<finderType> result;
        auto index = sources.begin();
        int totalCount = sources.count();
        int curTakeCount = 0;
        int limitedCount = TakeCount == -1 ? totalCount : TakeCount;
        while (curTakeCount < limitedCount && index != sources.end()) {

            result.push_back(*(++index));
            curTakeCount++;
        }
        return result;
    }
    ///in QList,take a Res type result for each finderType
    template<typename Res>
    static inline QList<Res> select (const QList<finderType>& sources,std::function<Res(finderType)> selectFunc)
    {
        QList<Res> res;
        for(auto i:sources)
        {
            res.push_back(selectFunc);
        }
        return res;
    }
    ///in arrays,task a Res type result for each finderType
    template<typename Res>
    static inline QList<Res> select (const finderType* sources,int sourcesCount,std::function<Res(finderType)> selectFunc)
    {
        QList<Res> res;
        for(int i = 0;i<sourcesCount;i++)
        {
            res.push_back(sources[i]);
        }
        return res;
    }


};

///
/// The QStringExtension class, implemented a bunch of methods to enhance operations with QString
///
class QStringExtension
{
public:
    ///check if file exist
    static inline bool isFileExist(const QString &str)
    {
        if (isNullOrEmpty(str))
            return false;
        QFile file(str);
        bool ex = file.exists();
        file.close();
        return ex;
    }
    ///check if string is empty or nullptr
    static inline bool isNullOrEmpty(const QString &str)
    {
        return str == nullptr || str.isNull() || str.isEmpty();
    }
    /// read text from file
    static inline QString readAllText(const QString &filePath, const char *encode = "utf-8")
    {

        QString result;
        if (isNullOrEmpty(filePath) || !isFileExist(filePath))
            return result;
        QFile curFile(filePath);
        if (!curFile.open(QIODevice::ReadOnly | QIODevice::Text))
            return result;
        QTextStream in(&curFile);
        in.setCodec(encode);
        while (!in.atEnd()) {
            result.append(in.readLine());
        }
        curFile.close();
        return result;
    }
    ///write text to file
    static inline void writeAllText(const QString &filePath, const QString &content)
    {

        QFile file(filePath);
        file.open(
            QIODevice::WriteOnly | QIODevice::Text);//if the file is not exist,file will automatic created when writing.
        QTextStream out(&file);
        out << content;
        file.close();
    }
    ///
/// a enum source to determine the option of QStringExtension::GetFileInfo
///
    enum QStringExtensionPathOptionMode
    {
        ///get file name,with extension
        FileName,
        ///get extension ,but doesn't have the '.' symbol
        FileExtension,
        ///get file name ,no extension
        FileNameWithoutExtension,
        ///get the parent directory of file
        ParentDir,
        /// get the absolute path of file
        AbsolutePath,
    };
    ///In the specified path,get infos by {mode}
    static inline QString getFileInfo(const QString &filePath, QStringExtensionPathOptionMode mode)
    {
        QString result;
        if (isNullOrEmpty(filePath) || !isFileExist(filePath))
            return result;
        QFileInfo file(filePath);
        switch (mode) {
        case FileExtension:
            result = file.completeSuffix();
            break;
        case FileName:
            result = file.fileName();
            break;
        case FileNameWithoutExtension:
            result = file.completeBaseName();
            break;
        case ParentDir:
            result = file.absoluteDir().path();
            break;
        case AbsolutePath:
            result = file.absoluteFilePath();
            break;
        }
        return result;
    }


};
}

/***A global pointer to access data,a type only has one pointer ***/
template<typename T>
T *GLOBAL_POINTER = nullptr;

template<typename T>
///set a global pointer to an object,return the previous GLOBAL_POINTER of the type,if there is already one exist
constexpr T *SET_POINTER(T *obj)
{
    auto pre = GLOBAL_POINTER<T>;
    GLOBAL_POINTER<T> = obj;
    return pre;
}
template<typename T>
///unset the global pointer of specific type
constexpr void UNSET_POINTER()
{
    GLOBAL_POINTER<T> = nullptr;
}
template<typename T>
///check if GLOBAL_POINTER<T> has been set
constexpr bool CHECK_POINTER()
{
    return GLOBAL_POINTER<T> != nullptr;
}

template<typename T>
///get the pointer
constexpr T *GET_POINTER()
{
    return GLOBAL_POINTER<T>;
}

#define SAFE_DELETE(arg) if(arg!=nullptr) {delete arg;arg = nullptr;}

#define POINTER_CHECK(arg) Q_ASSERT(arg==nullptr);

#endif //EXTENSIONMETHODS_H
