#include "Systems.h"
#include "Components.h"
#include "GL/glew.h"
#include "Game.h"
#include "Math.h"
#include "Rendering.h"

void InputSystem(entt::registry *registry, const Uint8 *keyState) {
  auto input = registry->view<const Input, Move>();
  input.each([keyState](const Input &input, Move &move) {
    float forwardSpeed = 0.0f;
    if (keyState[input.forwardKey]) {
      forwardSpeed += input.maxFwdSpeed;
    }
    if (keyState[input.backKey]) {
      forwardSpeed -= input.maxFwdSpeed;
    }
    move.forwardSpeed = forwardSpeed;

    float angularSpeed = 0.0f;
    if (keyState[input.clockwiseKey]) {
      angularSpeed += input.maxAngSpeed;
    }
    if (keyState[input.counterClockwiseKey]) {
      angularSpeed -= input.maxAngSpeed;
    }
    move.angularSpeed = angularSpeed;
  });
}

void ShootingSystem(entt::registry *registry, const Uint8 *keyState) {
  auto shooter = registry->view<Shoot, const Transform>();
  shooter.each([registry, keyState](Shoot &shoot, const Transform &pos) {
    if (keyState[shoot.shootKey] && shoot.shootCooldown < 0.0f) {
      auto laser = registry->create();
      registry->emplace<Laser>(laser, 1.0f, 11.0f);
      registry->emplace<Transform>(laser, pos.x, pos.y, pos.rot);
      registry->emplace<Move>(laser, 0.0f, 800.0f);
      Texture tex = GetTexture("Assets/Laser.png");
      registry->emplace<Texture>(laser, tex);
      registry->emplace<Shader>(
          laser, GetShader("Assets/Sprite.vert", "Assets/Sprite.frag"));
      shoot.shootCooldown = 0.5f;
    }
  });
}

void CollisionSystem(entt::registry *registry, float deltaTime) {
  registry->view<Shoot>().each(
      [deltaTime](Shoot &shoot) { shoot.shootCooldown -= deltaTime; });

  auto laserBeams = registry->view<Laser>();
  laserBeams.each([registry, deltaTime](const auto entity, Laser &laser) {
    laser.life -= deltaTime;
    if (laser.life < 0.0f) {
      registry->emplace<DestroyTag>(entity);
    }
  });

  auto laserBeamsCollision = registry->view<const Laser, const Transform>();
  auto asteroidCollision = registry->view<const Circle, const Transform>();

  for (auto [laserEntity, laser, laserPos] : laserBeamsCollision.each()) {
    for (auto [astEntity, circle, astPos] : asteroidCollision.each()) {
      Vector2 diff =
          Vector2(laserPos.x, laserPos.y) - Vector2(astPos.x, astPos.y);
      float distSq = diff.LengthSq();

      float radiiSq = laser.radius + circle.radius;
      radiiSq *= radiiSq;
      if (distSq <= radiiSq) {
        registry->emplace_or_replace<DestroyTag>(laserEntity);
        registry->emplace_or_replace<DestroyTag>(astEntity);
      }
    }
  }

  auto destroyEntities = registry->view<const DestroyTag>();
  registry->destroy(destroyEntities.begin(), destroyEntities.end());
}

void MovementSystem(entt::registry *registry, float deltaTime) {
  registry->view<const Move, Transform>().each(
      [deltaTime](const Move &move, Transform &_pos) {
        if (!Math::NearZero(move.angularSpeed)) {
          float rot = _pos.rot;
          rot += move.angularSpeed * deltaTime;
          _pos.rot = rot;
        }

        if (!Math::NearZero(move.forwardSpeed)) {
          Vector2 pos;
          pos.x = _pos.x;
          pos.y = _pos.y;
          Vector2 forward;
          forward.x = Math::Cos(_pos.rot);
          forward.y = Math::Sin(_pos.rot);

          pos += forward * move.forwardSpeed * deltaTime;

          if (pos.x < -512.0f) {
            pos.x = 510.0f;
          } else if (pos.x > 512.0f) {
            pos.x = -510.0f;;
          }

          if (pos.y < -384.0f) {
            pos.y = 382.0f;
          } else if (pos.y > 384.0f) {
            pos.y = -382.0f; 
          }

          _pos.x = pos.x;
          _pos.y = pos.y;
        }
      });
}

void RenderSystem(GameData *gd, entt::registry *registry) {
  glClearColor(0.86f, 0.86f, 0.86f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // each entity should have it's own mesh?
  auto vertexView = registry->view<VertexArray>();
  vertexView.each(
      [](VertexArray &vertexArray) { SetVerticesActive(vertexArray.arrayID); });

  auto view = registry->view<const Shader, const Texture, const Transform>();
  view.each([](const Shader &shader, const Texture &texture,
               const Transform &transform) {
    SetShaderActive(shader.shaderProgram);

    Matrix4 scaleMat =
        Matrix4::CreateScale(static_cast<float>(texture.texWidth),
                             static_cast<float>(texture.texHeight), 1.0f);

    Matrix4 worldTransform = Matrix4::CreateScale(texture.scale);
    worldTransform *= Matrix4::CreateRotationZ(transform.rot);
    worldTransform *=
        Matrix4::CreateTranslation(Vector3(transform.x, transform.y, 0.0f));
    Matrix4 world = scaleMat * worldTransform;
    SetMatrixUniform(shader, "uWorldTransform", &world);
    SetTextureActive(texture.textureID);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
  });

  SDL_GL_SwapWindow(gd->window);
}
