

class TitleScene {
	public:
		TitleScene();

		~TitleScene();

		void Initialize();

		void Update();

		void Draw();

		bool IsFinished() const {return finished_;}

	private:
		bool finished_ = false;
};