for (int i = 0; i < results.size(); i++) {
	bool tracker_update = results[i].first->update(color_matrix_comp, results[i].second.bounding_box);
	SPDLOG_INFO("UPDATED TRACKER");
	if (results[i].second.no_rc_counter > 30) {
		SPDLOG_INFO("Lost a tracked object {} - erasing", results[i].second.id);
		free_ids.push(results[i].second.id);
		if (results[i].second.distance > results[i].second.first_distance) {
			events.push_back(std::pair<std::string, long>(std::string("human out"), clock()));
		}
		else {
			events.push_back(std::pair<std::string, long>(std::string("human in"), clock()));
		}
		results.erase(results.begin() + i);
	}
	else {
		results[i].second.no_rc_counter++;

	}

	/// <summary>
/// Get all objects with their classes, distance and corresponding realtive locations within the frame
/// and clears them afterwards, so that they are not used again to avoid reusing the same result in the following frames
/// </summary>
//std::vector<ClassificationResult> get_and_clear_results()
//{
//	std::vector<ClassificationResult> to_send = { results_.begin(), results_.end() };

//	// Note that the clear function below is not concurrency-safe
//	results_.clear();
//	results_.shrink_to_fit();

//	SPDLOG_INFO("Got and cleared {} classification results", to_send.size());

//	return to_send;
//}
/// <summary>
/// Receives new frames and pushes assign these frames to object recognition models for object dectection
/// </summary>
/// <param name="color_matrix">color matrix obtained from the camera</param>
/// <param name="depth_matrix">depth matrix obtained from the camera</param>



/// <summary>
/// Gets the size of the list containing all object recognition models added so far
/// </summary>
/// <returns>the number of object recognition models added so far</returns>


//SPDLOG_INFO("STARTING KMEANS");

			////cv::imshow("depth_matrix", object_depth);
			////std::string empty = "";
			////std::cin >> empty;
			//object_depth.convertTo(object_depth, CV_32F);
			///*			if (!object_depth.isContinuous()) {
			//				object_depth = object_depth.clone();
			//			}*/
			//cv::Mat color_mask(object_depth.rows, object_depth.cols, CV_8UC3, cv::Scalar(255, 0, 255));
			//object_depth = object_depth.reshape(0, 1);

			//cv::kmeans(object_depth, 2, label, cv::TermCriteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 100, 0.01), 1, cv::KMEANS_RANDOM_CENTERS, centers);
			//cv::Mat mask = label.reshape(0, depth_matrix(object).rows);
			//mask.convertTo(mask, CV_8U);
			////cv::Mat distances = centers * depth_frame.get_units();
			////SPDLOG_INFO("{}, {}", distances.rows, distances.cols);


			//uchar center_object_pixel = mask.at<uchar>(mask.rows / 2, mask.cols / 2);
			//if (center_object_pixel == 0) {
			//	for (int i = 0; i < mask.rows; i++) {
			//		for (int j = 0; j < mask.cols; j++) {
			//			if (mask.at<uchar>(i, j) == 1) {
			//				mask.at<uchar>(i, j) = 0;
			//			}
			//			else {
			//				mask.at<uchar>(i, j) = 1;
			//			}
			//		}
			//	}
			//}



			///*	double minvalue;
			//	double maxvalue;

			//	cv::minMaxLoc(mask, &minvalue, &maxvalue);*/
			//	/*			SPDLOG_INFO("{}, {}", minvalue, maxvalue);*/

			//				//cv::imshow("mask",mask); 
			//color_mask.copyTo(color_matrix(object), mask);
			//SPDLOG_INFO("FINISHED KMEANS");


	/*	ml_controller_.new_frames(color_matrix, depth_matrix);

				SPDLOG_INFO("Prioritising results");
				auto prioritised_results = prioritiser_.prioritise(ml_controller_.get_and_clear_results());
				SPDLOG_INFO("Prioritised {} results", prioritised_results.size());
				for (int i = 0; i < prioritised_results.size(); i++) {
					SPDLOG_INFO(prioritised_results[i].to_string());
					SPDLOG_INFO("top_left: {} {}, right_bottom: {} {}", prioritised_results[i].top_left.x, prioritised_results[i].top_left.y, prioritised_results[i].bottom_right.x, prioritised_results[i].bottom_right.y);

				};*/
				/*float upixel[2];
				float vpixel[2];
				float upoint[3];
				float vpoint[3];

				std::vector<std::vector<float>> distance_matrix;
				if (prioritised_results.size() > 1) {
					for (int i = 0; i < prioritised_results.size(); i++) {
						for (int j = 0; j < prioritised_results.size(); j++)
						{
							auto object1 = prioritised_results[i];
							auto object2 = prioritised_results[j];
							upixel[0] = static_cast<float>((prioritised_results[i].bottom_left.x + prioritised_results[i].bottom_right.x) / 2);
							upixel[1] = static_cast<float>((prioritised_results[i].bottom_left.y + prioritised_results[i].top_left.y) / 2);
							vpixel[0] = static_cast<float>((prioritised_results[j].bottom_left.x + prioritised_results[j].bottom_right.x) / 2);
							vpixel[1] = static_cast<float>((prioritised_results[j].bottom_left.y + prioritised_results[j].top_left.y) / 2);
							rs2_deproject_pixel_to_point(upoint, &intr, upixel, prioritised_results[i].distance);
							rs2_deproject_pixel_to_point(vpoint, &intr, vpixel, prioritised_results[j].distance);

							float vertical_distance = sqrt(pow(upoint[0] - vpoint[0], 2.f) + pow(upoint[1] - vpoint[1], 2.f) + pow(upoint[2] - vpoint[2], 2.f))
								;

							SPDLOG_INFO("Vertical distance between object {} and {}: {}", prioritised_results[i].name, prioritised_results[j].name, vertical_distance);
							SPDLOG_INFO("Overlap between these objects: {}", overlap);
							if ((object1.name == "chair" && object2.name == "person") || (object1.name == "person" && object2.name == "chair")) {
								if (vertical_distance < 0.5 && overlap > 0.7) {
									SPDLOG_INFO("Person sitting on a chair");
								}
								else {
									SPDLOG_INFO("Person NOT on a chair");

								}
							}

						}
					}

				}*/
				//SPDLOG_INFO("Sending items to API");
				//api_controller_.new_items(prioritised_results);

				//SPDLOG_INFO("Sending frames to output stream");


				//output_stream_controller_.stream_to_windows(
				//	data.get_depth_frame().apply_filter(color_map), depth_matrix,
				//	data.get_color_frame(), color_matrix,
				//	prioritised_results);
