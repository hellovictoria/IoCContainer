#ifndef _IOCCONTAINER_H_
#define _IOCCONTAINER_H_
#include <string>
#include <typeinfo>
#include <map>
#include <mutex>
#include <iterator>
#include <memory>
#include <iostream>

class IOCContainer
{
    public:
        static IOCContainer* getInstance()
        {
            if(uniqueIns==NULL)
            {
                uniqueIns = new IOCContainer();
            }
            return uniqueIns;
        }

        template<class T>
        void Register(std::shared_ptr<T> t)
        {
            std::cout << "In func -- void Register(std::shared_ptr<T> t)"<<std::endl;
            const std::type_info* typeId=&typeid(T);
            Register(typeId->name(),t);
        }

        template<class T>
        void Register(const std::type_info* typeId,std::shared_ptr<T> t)
        {
            std::cout << "In func -- void Register(const std::type_info* typeId,std::shared_ptr<T> t)"<<std::endl;
            if(!typeId)
            {
                throw std::runtime_error("Invalid type id to register");
            }
            Register(typeId->name(),t);
        }

        template<class T>
        void Register(const std::string& id,std::shared_ptr<T> t)
        {
            std::cout << "In func -- void Register(const std::string& id,std::shared_ptr<T> t)"<<std::endl;
            std::cout <<"typeId->name=" <<id << std::endl;
            std::lock_guard<std::mutex> lock(m_mapMutex);
            std::map<std::string,std::shared_ptr<void>>::iterator iter=m_mapping.find(id);
            if(iter==m_mapping.end())
            {
                m_mapping[id]=t;
            }
        } 

        template<class T>
        std::shared_ptr<T> Resolve()
        {
            std::cout << "In func -- Resolve()" << std::endl;
            const std::type_info* typeId = &typeid(T);
            return Resolve<T>(typeId->name());
        }

        template<class T>
        std::shared_ptr<T> Resolve(const std::type_info* typeId)
        {
            std::cout << "In func -- Resolve(const std::type_info* typeId)" << std::endl;
            if(!typeId)
            {
                throw std::runtime_error("Invalid type id to resolve");
            }

            return Resolve<T>(typeId->name());
        }

        template<class T>
        std::shared_ptr<T> Resolve(const std::string& id)
        {
            std::cout << "In func -- Resolve(const std::string& id)" << std::endl;
            std::cout << "Id=" << id << std::endl;
            std::lock_guard<std::mutex> lock(m_mapMutex);
            std::map<std::string,std::shared_ptr<void>>::iterator iter=m_mapping.find(id);

            if(iter!=m_mapping.end())
            {
                return std::static_pointer_cast<T>(iter->second);
            }
            throw std::runtime_error("Could not locate type in IOC");
        }

        ~IOCContainer()
        {
            if(uniqueIns==NULL) return;
            delete uniqueIns;
            uniqueIns=NULL;
        }

    private:
        std::map<std::string,std::shared_ptr<void>> m_mapping;
        std::mutex m_mapMutex;

        static IOCContainer* uniqueIns;

        IOCContainer(){}
        IOCContainer(const IOCContainer&);
        void operator=(const IOCContainer&);
};

#endif
