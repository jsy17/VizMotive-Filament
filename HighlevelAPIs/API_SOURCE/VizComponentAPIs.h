#ifndef VIZCOMPONENTAPIS
#define VIZCOMPONENTAPIS

#pragma warning (disable : 4251)
#pragma warning (disable : 4819)
// FEngine Warnings
#pragma warning (disable : 4146)
#pragma warning (disable : 4068)
#pragma warning (disable : 4267)
#pragma warning (disable : 4244)

#define _ITERATOR_DEBUG_LEVEL 0 // this is for using STL containers as our standard parameters

#define __dojostatic extern "C" __declspec(dllexport)
#define __dojoclass class __declspec(dllexport)
#define __dojostruct struct __declspec(dllexport)

#define __FP (float*)&
#define VZRESULT int
#define VZ_OK 0
#define VZ_FAIL 1
#define VZ_JOB_WAIT 2
#define VZ_WARNNING 3

// std
#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include <set>
#include <any>
#include <list>
#include <memory>
#include <algorithm>
#include <chrono>
#include <functional>

using VID = uint32_t;
inline constexpr VID INVALID_VID = 0;
using TimeStamp = std::chrono::high_resolution_clock::time_point;

constexpr float VZ_PI = 3.141592654f;
constexpr float VZ_2PI = 6.283185307f;
constexpr float VZ_1DIVPI = 0.318309886f;
constexpr float VZ_1DIV2PI = 0.159154943f;
constexpr float VZ_PIDIV2 = 1.570796327f;
constexpr float VZ_PIDIV4 = 0.785398163f;

using uint = uint32_t;

namespace vzm
{
    template <typename ID> struct ParamMap {
    private:
        std::string __PM_VERSION = "LIBI_1.4";
        std::unordered_map<ID, std::any> __params;
    public:
        bool FindParam(const ID& param_name) {
            auto it = __params.find(param_name);
            return !(it == __params.end());
        }
        template <typename SRCV> bool GetParamCheck(const ID& key, SRCV& param) {
            auto it = __params.find(key);
            if (it == __params.end()) return false;
            param = std::any_cast<SRCV&>(it->second);
            return true;
        }
        template <typename SRCV> SRCV GetParam(const ID& key, const SRCV& init_v) const {
            auto it = __params.find(key);
            if (it == __params.end()) return init_v;
            return std::any_cast<const SRCV&>(it->second);
        }
        template <typename SRCV> SRCV* GetParamPtr(const ID& key) {
            auto it = __params.find(key);
            if (it == __params.end()) return NULL;
            return (SRCV*)&std::any_cast<SRCV&>(it->second);
        }
        template <typename SRCV, typename DSTV> bool GetParamCastingCheck(const ID& key, DSTV& param) {
            auto it = __params.find(key);
            if (it == __params.end()) return false;
            param = (DSTV)std::any_cast<SRCV&>(it->second);
            return true;
        }
        template <typename SRCV, typename DSTV> DSTV GetParamCasting(const ID& key, const DSTV& init_v) {
            auto it = __params.find(key);
            if (it == __params.end()) return init_v;
            return (DSTV)std::any_cast<SRCV&>(it->second);
        }
        void SetParam(const ID& key, const std::any& param) {
            __params[key] = param;
        }
        void RemoveParam(const ID& key) {
            auto it = __params.find(key);
            if (it != __params.end()) {
                __params.erase(it);
            }
        }
        void RemoveAll() {
            __params.clear();
        }
        size_t Size() {
            return __params.size();
        }
        std::string GetPMapVersion() {
            return __PM_VERSION;
        }

        typedef std::unordered_map<ID, std::any> MapType;
        typename typedef MapType::iterator iterator;
        typename typedef MapType::const_iterator const_iterator;
        typename typedef MapType::reference reference;
        iterator begin() { return __params.begin(); }
        const_iterator begin() const { return __params.begin(); }
        iterator end() { return __params.end(); }
        const_iterator end() const { return __params.end(); }
    };

    enum class SCENE_COMPONENT_TYPE // every component involves a transform and a name
    {
        // camera, light, actor component can have renderable resources technically
        SCENEBASE = 0,  // empty (only transform and name)
        CAMERA,
        LIGHT,
        ACTOR,
    };

    enum class RES_COMPONENT_TYPE
    {
        RESOURCE = 0,
        GEOMATRY,
        MATERIAL,
        MATERIALINSTANCE,
    };

    __dojostruct VzBaseComp
    {
        // DO NOT SET
        VID componentVID = INVALID_VID;
        TimeStamp timeStamp = {}; // will be automatically set 
        std::string originFrom;
        std::string type;

        // User data
        ParamMap<std::string> attributes;

        std::string GetName();
        void SetName(const std::string& name);
    };
    __dojostruct VzSceneComp : VzBaseComp
    {
        SCENE_COMPONENT_TYPE compType = SCENE_COMPONENT_TYPE::SCENEBASE;

        void GetWorldPosition(float v[3]);
        void GetWorldForward(float v[3]);
        void GetWorldRight(float v[3]);
        void GetWorldUp(float v[3]);
        void GetWorldTransform(float mat[16], const bool rowMajor = false);
        void GetWorldInvTransform(float mat[16], const bool rowMajor = false);

        void GetLocalTransform(float mat[16], const bool rowMajor = false);
        void GetLocalInvTransform(float mat[16], const bool rowMajor = false);

        // local transforms
        void SetTransform(const float s[3] = nullptr, const float q[4] = nullptr, const float t[3] = nullptr, const bool additiveTransform = false);
        void SetMatrix(const float value[16], const bool additiveTransform = false, const bool rowMajor = false);

        VID GetParent();
        std::vector<VID> GetChildren();
        VID GetScene();
    };
    __dojostruct VzResource : VzBaseComp
    {
        RES_COMPONENT_TYPE compType = RES_COMPONENT_TYPE::RESOURCE;
    };
}

#endif
