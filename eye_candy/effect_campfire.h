
#ifndef EFFECT_CAMPFIRE_H
#define EFFECT_CAMPFIRE_H

// I N C L U D E S ////////////////////////////////////////////////////////////

#include "eye_candy.h"

namespace ec
{

// C L A S S E S //////////////////////////////////////////////////////////////

class CampfireParticle : public Particle
{
public:
  CampfireParticle(Effect* _effect, ParticleMover* _mover, const Vec3 _pos, const Vec3 _velocity, const float _scale, const float _sqrt_scale, const int _state, const u_int16_t _LOD);
  ~CampfireParticle() {}
  
  virtual bool idle(const u_int64_t delta_t);
  virtual void draw(const u_int64_t usec);
  virtual GLuint get_texture(const u_int16_t res_index);
  virtual light_t estimate_light_level() const { return 0.002; };	// We don't want the particle system lights to be used on the pos, since it will assumedly already have one.

  coord_t size_max;  
};

class CampfireBigParticle : public Particle
{
public:
  CampfireBigParticle(Effect* _effect, ParticleMover* _mover, const Vec3 _pos, const Vec3 _velocity, const float _sqrt_scale, const u_int16_t _LOD);
  ~CampfireBigParticle() {}
  
  virtual bool idle(const u_int64_t delta_t);
  virtual GLuint get_texture(const u_int16_t res_index);
  virtual light_t estimate_light_level() const { return 0.003; };	// Like above
  virtual bool deletable() { return false; };
};

class CampfireEffect : public Effect
{
public: 
  CampfireEffect(EyeCandy* _base, bool* _dead, Vec3* _pos, const std::vector<ec::Obstruction*> _obstructions, const float _scale, const u_int16_t _LOD);
  ~CampfireEffect(); 
  
  virtual EffectEnum get_type() { return EC_CAMPFIRE; };
  bool idle(const u_int64_t usec);

  GradientMover* mover;
  ParticleMover* stationary;
  ParticleSpawner* spawner;
  int big_particles;
  float scale;
  float sqrt_scale;
};

///////////////////////////////////////////////////////////////////////////////

}	// End namespace ec

#endif	// defined EFFECT_CAMPFIRE_H