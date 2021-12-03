#ifndef BOUDNING_BOX_HPP
#define BOUDNING_BOX_HPP

#include <glm/vec3.hpp>

namespace GL3
{
/**
 * @brief Generic 3D axis aligned bounding box class.
 */
class BoundingBox
{
 public:
    /**
     * @brief Construct a new Bounding Box object
     */
    BoundingBox();

    /**
     * @brief Destroy the Bounding Box object
     */
    ~BoundingBox() = default;

    /**
     * @brief Default copy constructor
     * @param bb other bounding box instance for copying
     */
    BoundingBox(const BoundingBox& bb);

    /**
     * @brief Default copy assignment constructor
     * @param bb other bounding box instance for copying
     * @return BoundingBox& return reference of this instance
     */
    BoundingBox& operator=(const BoundingBox& bb);

    /**
     * @brief Merge the new vec3 point
     */
    void Merge(glm::vec3 newPoint);

    /**
     * @brief Merge the bounding box
     * @param bb other bounding box for merging
     */
    void Merge(const BoundingBox& bb);

    /**
     * @brief Reset the bounding box
     */
    void Reset();

    /**
     * @brief Get the Lower Corner object
     * @return glm::vec3 lower corner position
     */
    [[nodiscard]] inline glm::vec3 GetLowerCorner() const
    {
        return _lowerCorner;
    }

    /**
     * @brief Get the Upper Corner object
     * @return glm::vec3 upper corner position
     */
    [[nodiscard]] inline glm::vec3 GetUpperCorner() const
    {
        return _upperCorner;
    }

 private:
    glm::vec3 _lowerCorner;
    glm::vec3 _upperCorner;
    bool _bFirstMerge;
};

};  // namespace GL3

#endif  //! end of BoundingBox.hpp