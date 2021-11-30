#ifndef SEQUENCE_OBJ_HPP
#define SEQUENCE_OBJ_HPP

#include <GL3/GLTypes.hpp>
#include <Common/GLTFScene.hpp>

namespace GL3
{
/**
 * @brief .
 */
class SequenceObj : public Common::GLTFScene
{
 public:
    /**
     * @brief Construct a new SequenceObj object
     */
    SequenceObj();

    /**
     * @brief Destroy the SequenceObj object
     */
    ~SequenceObj();

 private:
};

};  // namespace GL3

#endif  //! end of SequenceObj.hpp